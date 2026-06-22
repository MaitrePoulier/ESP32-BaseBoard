#include "i2s.h"
#include "driver/i2s.h"
#include "General.h"
#include "SD_MMC.h"

void i2s_init(uint32_t sample_rate)
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = sample_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 512,
        .use_apll = true,   // better audio clock
        .tx_desc_auto_clear = true
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_SD,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void AudioPlayTask(void *pvParameters)
{
    // Pass the file path string through the task parameter
    const char *path = (const char *)pvParameters;
    
    Serial.printf("Playing %s in background...\r\n",path);
    File file = SD_MMC.open(path);
    if (!file) {
        Serial.printf("Failed to open file\r\n");
        Serial.printf("Try 'SD' command before 'wav'\r\n> ");
        vTaskDelete(NULL);
        return;
    }

    // --- Read WAV header ---
    uint8_t header[44];
    file.read(header, 44);

    uint32_t sample_rate = *(uint32_t*)(header + 24);
    uint16_t bits_per_sample = *(uint16_t*)(header + 34);
    uint16_t channels = *(uint16_t*)(header + 22);

    Serial.printf("SampleRate: %lu\r\n", sample_rate);
    Serial.printf("Bits: %u\r\n", bits_per_sample);
    Serial.printf("Channels: %u\r\n", channels);

    if (bits_per_sample != 16) {
        Serial.printf("Only 16-bit WAV supported\r\n");
        file.close();
        vTaskDelete(NULL);
        return;
    }
    //Normally we have no more to print on the screen, so we can add the '> ' to 
    //  look like we are ready to received a now command
    Serial.printf("> ");

    i2s_init(sample_rate);

    // --- Stream audio ---
    uint8_t buffer[1024];
    size_t bytes_written;

    while (file.available()) {
        int bytes_read = file.read(buffer, sizeof(buffer));

        i2s_write(I2S_NUM_0, buffer, bytes_read, &bytes_written, portMAX_DELAY);
    }

    file.close();
    i2s_driver_uninstall(I2S_NUM_0);

    Serial.printf("Playback done\r\n> ");
    vTaskDelete(NULL);
}

void play_wav(const char *path)
{
    // Create the task on Core 1 (leaves Core 0 free for Wi-Fi/BT)
    // We give it a healthy 4096 bytes of stack because SD_MMC file operations can be memory heavy.
    xTaskCreatePinnedToCore(
        AudioPlayTask,
        "AudioTask",
        4096,
        (void*)path,       // Pass the path string pointer to the task
        5,                 // Medium priority
        NULL,
        1                  // Pin to Core 1
    );
}