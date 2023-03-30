#pragma once

#include <stdint.h>
#include <string>

#include <App/App.hpp>

#define qrcodegen_MAX_VERSION   6
#define QR_MEM_QR_CODE_SIZE     (qrcodegen_BUFFER_LEN_FOR_VERSION(qrcodegen_MAX_VERSION))
#define QR_MEM_TEMP_SIZE        (qrcodegen_BUFFER_LEN_FOR_VERSION(qrcodegen_MAX_VERSION))
#define QR_MEM_SIZE             (QR_MEM_QR_CODE_SIZE) // + QR_MEM_TEMP_SIZE)

class QrCodeView : public App  {
    public:
        QrCodeView() : redraw_(true), qrcode_mem_(nullptr), qrcode_temp_mem_(nullptr) {};
        virtual ~QrCodeView() {};

        void set_title(const std::string &title) {
            redraw_ = true;
            title_ = title;
        };
        void set_text(const std::string &text) {
            redraw_ = true;
            text_ = text;
        };

        virtual void begin(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

    private:
        void draw_title(Adafruit_GFX &gfx);
        void draw_qrcode(Adafruit_GFX &gfx);

        bool redraw_;
        std::string title_;
        std::string text_;
        uint8_t *qrcode_mem_;
        uint8_t *qrcode_temp_mem_;
};
