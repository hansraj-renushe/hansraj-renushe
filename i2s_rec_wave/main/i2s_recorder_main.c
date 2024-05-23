#include <stdio.h>
#include "driver/i2s.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"

#define I2S_SAMPLE_RATE         (44100)
#define I2S_NUM                 (0)
#define I2S_BCK_PIN             (14)
#define I2S_WS_PIN              (15)
#define I2S_SD_PIN              (13)
#define RECORDING_DURATION_SEC  3
#define SAMPLE_BUFFER_SIZE      (RECORDING_DURATION_SEC * I2S_SAMPLE_RATE * sizeof(int16_t))

#define WAV_HEADER_SIZE         44

static const char *TAG = "I2S_MIC";

void app_main(void)
{
	esp_vfs_spiffs_conf_t conf = {
	    .base_path = "/spiffs",
	    .partition_label = NULL, // Use default partition label
	    .max_files = 5,
	    .format_if_mount_failed = true
	};
	esp_err_t ret = esp_vfs_spiffs_register(&conf);
	if (ret != ESP_OK) {
	    ESP_LOGE(TAG, "Failed to mount SPIFFS partition (%s)", esp_err_to_name(ret));
	    return;
	}

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

    // Allocate memory buffer
    int16_t *sample_buffer = (int16_t *)malloc(SAMPLE_BUFFER_SIZE);
    if (sample_buffer == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory buffer");
        return;
    }

    size_t total_bytes_read = 0;
    size_t bytes_read;

    // Start recording
    TickType_t start_time = xTaskGetTickCount();
    while (1) {
        // Read data from the I2S interface
        i2s_read(I2S_NUM, (void *)(sample_buffer + total_bytes_read / sizeof(int16_t)),
                 SAMPLE_BUFFER_SIZE - total_bytes_read, &bytes_read, portMAX_DELAY);
        total_bytes_read += bytes_read;

        // Check if recording duration has reached
        TickType_t current_time = xTaskGetTickCount();
        if ((current_time - start_time) >= (RECORDING_DURATION_SEC * 1000 / portTICK_PERIOD_MS)) {
            ESP_LOGI(TAG, "Recording finished. Total bytes read: %d", total_bytes_read);
            break;
        }
    }

    // Write audio data to WAV file
    FILE *file = fopen("/spiffs/recording.wav", "wb");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to create WAV file");
        free(sample_buffer);
        return;
    }

    // Write WAV header
    uint32_t data_size = total_bytes_read;
    uint32_t file_size = data_size + WAV_HEADER_SIZE - 8;
    fwrite("RIFF", 1, 4, file);
    fwrite(&file_size, 4, 1, file);
    fwrite("WAVEfmt ", 1, 8, file);
    uint32_t fmt_size = 16;
    fwrite(&fmt_size, 4, 1, file);
    uint16_t audio_format = 1; // PCM
    fwrite(&audio_format, 2, 1, file);
    uint16_t num_channels = 1; // Mono
    fwrite(&num_channels, 2, 1, file);
//    fwrite(&I2S_SAMPLE_RATE, 4, 1, file);
    uint32_t byte_rate = I2S_SAMPLE_RATE * 2;
    fwrite(&byte_rate, 4, 1, file);
    uint16_t block_align = 2;
    fwrite(&block_align, 2, 1, file);
    uint16_t bits_per_sample = 16;
    fwrite(&bits_per_sample, 2, 1, file);
    fwrite("data", 1, 4, file);
    fwrite(&data_size, 4, 1, file);

    // Write audio data
    fwrite(sample_buffer, total_bytes_read, 1, file);
    fclose(file);

    // Free memory buffer
    free(sample_buffer);

    ESP_LOGI(TAG, "WAV file written: /spiffs/recording.wav");
}
