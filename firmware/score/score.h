#ifndef _H_SCORE_H_
#define _H_SCORE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*read_mac_t)(unsigned char *);
typedef void(*copy_t)(void *, const void *, unsigned int);
typedef void(*rng_t)(void *, unsigned int);
typedef void(*crc_t)(const void *, unsigned int, unsigned int *);
typedef void(*enc_t)(const void *, void *, unsigned int);

void set_kx_size(unsigned int size);
void set_k(const char *key);
void set_k2(const char *key);
void set_rm(read_mac_t fn);
void set_copy(copy_t fn);
void set_rng(rng_t fn);
void set_crc(crc_t fn);
void set_enc(enc_t fn);

#define SCORE_CODE_SNAKE    (0x12)
#define SCORE_CODE_TETRIS   (0x13)
const char *get_surl(char *buf, unsigned int size, const char *url, unsigned char game, unsigned int code);

#ifdef __cplusplus
}
#endif

#endif
