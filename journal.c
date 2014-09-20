/* This was fun enough that I might expand it into its own
 * project so I'll write the header assuming as much.
 *
 * Distributed under the ISC License.
 * Copyright (c) 2014, Anthony Garcia <anthony@lagg.me>
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* I admit, they should mention this in the spec. This is literally the only thing I had trouble with. */
#define ALIGN(x) ((x + 7ULL) & ~7ULL)

typedef uint8_t sd128[16];

typedef enum {
    UNUSED,
    DATA,
    FIELD,
    ENTRY,
    DATA_HASH_TABLE,
    FIELD_HASH_TABLE,
    ENTRY_ARRAY,
    TAG,
    _MAX
} object_type;

/* LP... Lennart Poettering. OH MY GOD THE MARK OF THE BEAST RUN */
const char *journal_signature = "LPKSHHRH";

typedef struct header_t {
    uint8_t signature[8];
    uint32_t compatible_flags;
    uint32_t incompatible_flags;
    uint8_t state;
    uint8_t reserved[7];
    sd128 file_id;
    sd128 machine_id;
    sd128 boot_id;
    sd128 seqnum_id;
    uint64_t header_size;
    uint64_t arena_size;
    uint64_t data_hash_table_offset;
    uint64_t data_hash_table_size;
    uint64_t field_hash_table_offset;
    uint64_t field_hash_table_size;
    uint64_t tail_object_offset;
    uint64_t n_objects;
    uint64_t n_entries;
    uint64_t tail_entry_seqnum;
    uint64_t head_entry_seqnum;
    uint64_t entry_array_offset;
    uint64_t head_entry_realtime;
    uint64_t tail_entry_realtime;
    uint64_t tail_entry_monotonic;
    uint64_t n_data;
    uint64_t n_fields;
    uint64_t n_tags;
    uint64_t n_entry_arrays;
} header;

#define OBJECT_SIZE 16
typedef struct object_header_t {
    uint8_t type;
    uint8_t flags;
    uint8_t reserved[6];
    uint64_t size;
    /* Update OBJECT_SIZE when fields are added */
    uint8_t *payload;
} object_header;

#define DATA_SIZE 48
typedef struct data_object_t {
    uint64_t hash;
    uint64_t next_hash_offset;
    uint64_t next_field_offset;
    uint64_t entry_offset;
    uint64_t entry_array_offset;
    uint64_t n_entries;
    /* Update DATA_SIZE when fields are added */
    uint8_t *payload;
} data_object;

typedef struct field_object_t {
    uint64_t hash;
    uint64_t next_hash_offset;
    uint64_t head_data_offset;
} field_object;

typedef struct entry_item_t {
    uint64_t object_offset;
    uint64_t hash;
} entry_item;

typedef struct entry_object_t {
    uint64_t seqnum;
    uint64_t realtime;
    uint64_t monotonic;
    sd128 boot_id;
    uint64_t xor_hash;
} entry_object;

typedef struct hash_item_t {
    uint64_t head_hash_offset;
    uint64_t tail_hash_offset;
} hash_item;

typedef struct hash_table_object_t {
    hash_item *items;
} hash_table_object;

typedef struct entry_array_object_t {
    uint64_t next_entry_array_offset;
} entry_array_object;

/* End structs with dynamic payloads */

typedef struct tag_object_t {
    uint64_t seqnum;
    uint64_t epoch;
    uint8_t tag[32];
} tag_object;

/* fstream is a pointer to the file stream to read from. This is
 * required since the common object header is basically the
 * encapsulating header, think IP packets wrapping UDP
 *
 * returns a pointer to obj if successful, NULL otherwise
 *
 * Note that the payload struct member points to an f(n)+1 byte zeroed
 * block of memory that the raw payload gets read into and should be freed when
 * you're done with it. Where f(n) is the 64 bit aligned encapsulated object size
 */
object_header *read_object(FILE *fstream, object_header *obj) {
    size_t read = 0;
    memset(obj, 0, sizeof(object_header));

    if ((read = fread(obj, 1, OBJECT_SIZE, fstream)) != OBJECT_SIZE) {
        fprintf(stderr, "Couldn't read common object header. Expected %d, got %d\n", OBJECT_SIZE, read);
        return NULL;
    } else if (obj->flags != 0) {
        fprintf(stderr, "Common object is not raw object type, XZ isn't supported yet due to 3rd party dependencies I want to avoid for now.\n");
        return NULL;
    }

    if (obj->size > 0) {
        uint64_t payload_size = ALIGN(obj->size) - OBJECT_SIZE;

        obj->payload  = (uint8_t *)calloc(payload_size + 1, 1);

        if (fread(obj->payload, 1, payload_size, fstream) != payload_size) {
            fprintf(stderr, "Couldn't read all %llu bytes of common payload\n", payload_size);
            free(obj->payload);
            return NULL;
        }
    }

    return obj;
}

/* common_payload is the payload after the common object header.
 * obj is a pointer to a data_object where the object will be stored
 *
 * returns pointer to obj if successful, NULL otherwise
 * */
data_object *read_data_object(uint8_t *common_payload, data_object *obj) {
    uint8_t *pptr = common_payload;

    memcpy(obj, pptr, DATA_SIZE);

    pptr += DATA_SIZE;

    obj->payload = pptr;

    return obj;
}

int main() {
    FILE *jstream = stdin;

    if (!jstream) {
        fprintf(stderr, "Ded journal\n");
        return 1;
    } else {
        header jheader;

        fread(&jheader, sizeof(header), 1, jstream);

        if (strncmp((char *)jheader.signature, journal_signature, strlen(journal_signature)) != 0) {
            fprintf(stderr, "This doesn't appear to be a systemd journal file.\n");
            return 1;
        }

        printf("Entries: %llu\n", jheader.n_entries);

        do {
            size_t main_payload_size = 0;
            object_header oheader;

            if (!read_object(jstream, &oheader)) {
                break;
            }

            if (oheader.type == UNUSED) {
                /* Probably hit unused space. Journal files are allocated in blocks so when you hit this it's pretty much EOF */
                fprintf(stderr, "Reached end of journal stream without issue\n");
                break;
            }

            /* Size members count the size of the payload INCLUDING the common header, this doesn't */
            main_payload_size = ALIGN(oheader.size) - OBJECT_SIZE;

            switch (oheader.type) {
                case DATA:
                    {
                        data_object obj;

                        /* Not going to do anything interesting here since
                         * data fields without the context of the entry list
                         * are basically just there to build the hash table
                         * early. But since I'm doing it via seeks there's
                         * no reason to do much else yet.
                         */
                        read_data_object(oheader.payload, &obj);
                    }
                    break;
                case ENTRY:
                    {
                        uint8_t *pptr = oheader.payload;
                        size_t sz = 0;
                        uint64_t current_offset = 0;
                        entry_object obj;

                        const size_t esize = sizeof(entry_object);
                        const size_t isize = sizeof(entry_item);

                        /* What I'm going to be doing with seeks here is filthy and slow and really
                         * should be done with hash tables, but I want to get it working first.
                         */
                        current_offset = ftell(jstream);

                        memcpy(&obj, pptr, esize);

                        pptr += esize;

                        printf("\nEntry sequence %llu\n", obj.seqnum);

                        for (sz = (main_payload_size - esize) / isize; sz > 0; --sz) {
                            entry_item item;
                            object_header entry_header;

                            memcpy(&item, pptr, isize);

                            /*printf("  %llu\n", item.object_offset);*/

                            fseek(jstream, item.object_offset, SEEK_SET);

                            if (read_object(jstream, &entry_header)) {
                                data_object entry_data;

                                if (entry_header.type == DATA) {
                                    read_data_object(entry_header.payload, &entry_data);

                                    /* Works because there's always 1 extra byte at the end of the main payload
                                     * acting as null terminator
                                     */
                                    printf("    %s\n", entry_data.payload);
                                } else {
                                    fprintf(stderr, "    Waht. Data entry list has entry that isn't of the data type. It's %d\n", entry_header.type);
                                }

                                free(entry_header.payload);
                            }

                            pptr += isize;
                        }

                        fseek(jstream, current_offset, SEEK_SET);
                    }
                    break;
                default:
                    fprintf(stderr, "Unimplemented header type: %d\n", oheader.type);
                    break;
            }

            free(oheader.payload);
        } while (!feof(jstream));
    }

    if (jstream != stdin) {
        fclose(jstream);
    }

    return 0;
}
