//
//  main.c
//  DynamicTensorProject
//
//  Created by Ayşe Yılmaz on 15.02.2026.
//
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

//TENSOR TANIMI


typedef enum {
    TENSOR_FLOAT32,
    TENSOR_INT16,
    TENSOR_INT8
} TensorDataType;

typedef union {
    float*   f32_ptr;
    int16_t* i16_ptr;
    int8_t*  i8_ptr;
} TensorData;

typedef struct {
    TensorData data;
    TensorDataType type;
    size_t rows;
    size_t cols;
    size_t size;
} Tensor;

//TENSOR OLUŞTURMA
  

Tensor create_tensor(TensorDataType type, size_t rows, size_t cols) {
    Tensor t;
    t.type = type;
    t.rows = rows;
    t.cols = cols;
    t.size = rows * cols;

    switch (type) {
        case TENSOR_FLOAT32:
            t.data.f32_ptr = (float*)malloc(t.size * sizeof(float));
            break;
        case TENSOR_INT16:
            t.data.i16_ptr = (int16_t*)malloc(t.size * sizeof(int16_t));
            break;
        case TENSOR_INT8:
            t.data.i8_ptr  = (int8_t*)malloc(t.size * sizeof(int8_t));
            break;
    }

    return t;
}

// BELLEK TEMİZLEME
   
void destroy_tensor(Tensor* t) {
    if (!t) return;

    switch (t->type) {
        case TENSOR_FLOAT32:
            free(t->data.f32_ptr);
            break;
        case TENSOR_INT16:
            free(t->data.i16_ptr);
            break;
        case TENSOR_INT8:
            free(t->data.i8_ptr);
            break;
    }
}


  /*  FLOAT32 → INT8 QUANTIZATION
       (DEBUG DESTEKLİ     */

void quantize_to_int8(Tensor* tensor, float scale) {

    if (tensor->type != TENSOR_FLOAT32) {
        printf("Hata: Sadece FLOAT32 tensörler quantize edilir.\n");
        return;
    }

    float* old_data = tensor->data.f32_ptr;
    int8_t* new_data = (int8_t*)malloc(tensor->size);

    printf("\n=== DEBUG: INT8 Quantization Basladi ===\n");

    for (size_t i = 0; i < tensor->size; i++) {

        float original = old_data[i];

        // Ölçekleme
        float scaled = original / scale;

        // Yuvarlama
        float rounded = roundf(scaled);

        // Clamping
        if (rounded > 127) rounded = 127;
        if (rounded < -128) rounded = -128;

        // Atama
        new_data[i] = (int8_t)rounded;

        // DEBUG ÇIKTISI
            
        printf("i=%zu | Float=%.3f | Scaled=%.3f | Rounded=%.3f | Int8=%d\n",
               i, original, scaled, rounded, new_data[i]);
    }

    size_t old_size = tensor->size * sizeof(float);
    size_t new_size = tensor->size * sizeof(int8_t);

    free(old_data);

    tensor->data.i8_ptr = new_data;
    tensor->type = TENSOR_INT8;

    printf("Tasarruf: %zu byte (%%%0.0f)\n",
           old_size - new_size,
           ((float)(old_size - new_size) / old_size) * 100);

    printf("=== DEBUG BITTI ===\n");
}

// FLOAT32 → INT16
  

void quantize_to_int16(Tensor* tensor, float scale) {

    if (tensor->type != TENSOR_FLOAT32) return;

    float* old_data = tensor->data.f32_ptr;
    int16_t* new_data = (int16_t*)malloc(tensor->size * sizeof(int16_t));

    for (size_t i = 0; i < tensor->size; i++) {

        float val = roundf(old_data[i] / scale);

        if (val > 32767) val = 32767;
        if (val < -32768) val = -32768;

        new_data[i] = (int16_t)val;
    }

    free(old_data);

    tensor->data.i16_ptr = new_data;
    tensor->type = TENSOR_INT16;
}

// TENSOR YAZDIRMA
   
void print_tensor(Tensor* t) {

    printf("\nTensor (%zux%zu):\n", t->rows, t->cols);

    for (size_t r = 0; r < t->rows; r++) {
        for (size_t c = 0; c < t->cols; c++) {

            size_t i = r * t->cols + c;

            switch (t->type) {
                case TENSOR_FLOAT32:
                    printf("%6.2f ", t->data.f32_ptr[i]);
                    break;
                case TENSOR_INT16:
                    printf("%6d ", t->data.i16_ptr[i]);
                    break;
                case TENSOR_INT8:
                    printf("%6d ", t->data.i8_ptr[i]);
                    break;
            }
        }
        printf("\n");
    }
}


  // MAIN — DEMO


int main(int argc, const char * argv[]) {

    // 3x3 FLOAT32 tensor
    Tensor t = create_tensor(TENSOR_FLOAT32, 3, 3);

    // Örnek veri doldur
    for (size_t i = 0; i < t.size; i++) {
        t.data.f32_ptr[i] = (float)i * 1.25f;
    }

    printf("Orijinal FLOAT32 Tensor:");
    print_tensor(&t);

    //  INT8 Quantization Demo
    quantize_to_int8(&t, 0.1f);

    print_tensor(&t);

    destroy_tensor(&t);

    return 0;
}

