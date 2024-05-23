#include <stdio.h>
#include "driver/i2s.h"
#include "esp_system.h"
#include "esp_log.h"

#define I2S_SAMPLE_RATE     (44100)
#define I2S_NUM             (0)
#define I2S_BCK_PIN         (14)
#define I2S_WS_PIN          (15)
#define I2S_SD_PIN          (13)

static const char *TAG = "I2S_MIC";

void app_main(void)
{
    // I2S configuration
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    // I2S pin configuration
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD_PIN
    };

    // Install and start I2S driver
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM, &pin_config);
    i2s_set_clk(I2S_NUM, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);

    size_t bytes_read;
    int16_t samples[64];

    while (1) {
        // Read data from the I2S interface
        i2s_read(I2S_NUM, samples, sizeof(samples), &bytes_read, portMAX_DELAY);

        // Print the samples to Serial
        for (int i = 0; i < bytes_read / sizeof(int16_t); i++) {
            ESP_LOGI(TAG, "Sample %d: %d", i, samples[i]);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // Small delay to avoid flooding the Serial output
    }
}
