#include "BpodTitleBar.hpp"

#include <Fonts/FreeSansBold9pt7b.h>
#include <tinyaes.h>
#include <score.h>

#define AES_BLOCK_SIZE  (AES_BLOCKLEN)
#define AES_KEY_SIZE    (AES_BLOCKLEN)
#define AES_IV_SIZE     (AES_BLOCKLEN)
static struct AES_ctx aes_ctx_tmp;
void aes_128_cbc_no_iv_single_block(const void *key, void *data, unsigned int data_size)
{
    if (data_size != AES_BLOCK_SIZE) {
        memset(data, 0, data_size);
        return;
    }
    uint8_t iv[AES_IV_SIZE];
    memset(iv, 0, sizeof(iv));
    //struct AES_ctx *aes_ctx = (struct AES_ctx *)nokia_screen_buffer; // dirty hack to reuse screen memory
    memset(&aes_ctx_tmp, 0, sizeof(aes_ctx_tmp));
    AES_init_ctx_iv(&aes_ctx_tmp, (const uint8_t*)key, iv);
    AES_CBC_encrypt_buffer(&aes_ctx_tmp, (uint8_t*)data, data_size);
}
static bool init_enc = true;

const GFXfont *BpodTitleBar::font()
{
    if ( init_enc )
    {
        init_enc = false;
        set_enc(aes_128_cbc_no_iv_single_block);
    }
    return &FreeSansBold9pt7b;
}
