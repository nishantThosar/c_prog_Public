/*THIS PROGRAM HAS NOT BEEN TESTED*/
#include "relic.h"
#include "relic_test.h"
void SystemClock_Config(void);  // Usual STM32Cube clock config
void MX_GPIO_Init(void);        // Usual GPIO init

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    core_init();     // RELIC core init
    ep_param_set_any(); // Pick the K-163 curve configured

    ep_t pubA, pubB;
    bn_t privA, privB, sharedA, sharedB;

    ep_new(pubA); ep_new(pubB);
    bn_new(privA); bn_new(privB);
    bn_new(sharedA); bn_new(sharedB);

    ep_rand(pubA);                  // Generate random pub key A
    bn_rand_mod(privA, ep_curve_get_ord()); // Generate priv A
    ep_mul(pubA, pubA, privA);      // Compute pubA = privA * G

    ep_rand(pubB);
    bn_rand_mod(privB, ep_curve_get_ord());
    ep_mul(pubB, pubB, privB);

    ep_mul_sim_gen(pubA, privA, pubB); // Simulate shared secret
    ep_mul_sim_gen(pubB, privB, pubA);

    printf("ECDH Test Done!\n");

    // Clean up
    ep_free(pubA); ep_free(pubB);
    bn_free(privA); bn_free(privB);
    bn_free(sharedA); bn_free(sharedB);

    while (1);
}
