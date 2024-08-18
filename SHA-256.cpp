#include <iostream>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <curl/curl.h>

const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

uint32_t rightRotate(uint32_t value, unsigned int count) {
    return (value >> count) | (value << (32 - count));
}

void sha256_transform(uint32_t state[8], const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3],
             e = state[4], f = state[5], g = state[6], h = state[7];

    uint32_t w[64];
    for (int i = 0; i < 64; i++) {
        if (i < 16) {
            // Prepare the message schedule
            w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | (block[i * 4 + 2] << 8) | block[i * 4 + 3];
        } else {
            uint32_t s0 = rightRotate(w[i - 15], 7) ^ rightRotate(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rightRotate(w[i - 2], 17) ^ rightRotate(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t s0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
        uint32_t s1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);

        uint32_t temp1 = h + s1 + ch + k[i] + w[i];
        uint32_t temp2 = s0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

// SHA-256 main function
void sha256(const uint8_t *data, size_t length, uint8_t hash[32]) {
    uint32_t state[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Calculate padded data length
    uint64_t bitLength = length * 8;
    size_t paddedLength = ((length + 8) / 64 + 1) * 64;
    uint8_t *paddedData = new uint8_t[paddedLength];
    std::memcpy(paddedData, data, length);
    paddedData[length] = 0x80;
    std::memset(paddedData + length + 1, 0, paddedLength - length - 9);
    for (int i = 0; i < 8; i++) {
        hash[i * 4] = (state[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (state[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (state[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = state[i] & 0xFF;
    }
    delete[] paddedData;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    if (realsize + (*(size_t *)userp) >= 10000000) {
        std::cerr << "File too large." << std::endl;
        return 0;
    }
    std::memcpy((char *)buffer + (*(size_t *)userp), buffer, realsize);
    *(size_t *)userp += realsize;
    return realsize;
}

void download_data(const char *url, char *buffer, size_t *size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl." << std::endl;
        exit(1);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Failed to download data: " << curl_easy_strerror(res) << std::endl;
        exit(1);
    }

    curl_easy_cleanup(curl);
}

int main() {
    char url[] = "https://quod.lib.umich.edu/cgi/r/rsv/rsv-idx?type=DIV1&byte=4697892";
    char data[10000000] = {0};
    size_t size = 0;
    download_data(url, data, &size);

    uint8_t hash[32];
    sha256(reinterpret_cast<const uint8_t *>(data), size, hash);

    for (int i = 0; i < 32; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    std::cout << std::endl;

    return 0;
}
