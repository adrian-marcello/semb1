// Ádrian Marcello Mendes Dias - UFU
// arquivo de inicialização
// declara e inicializa stack
// declara e inicializa tabela de vetores de interrupção
// código da reset_handler
// outras exception handlers

#include <stdint.h>

// protótipo de funções
void reset_handler(void);
void nmi_handler       (void) __attribute__ ((weak, alias("default_handler")));
void hardfault_handler (void) __attribute__ ((weak, alias("default_handler")));
void memmanage_handler (void) __attribute__ ((weak, alias("default_handler")));
void busfault_handler  (void) __attribute__ ((weak, alias("default_handler")));
void usagefault_handler(void) __attribute__ ((weak, alias("default_handler")));
void svc_handler       (void) __attribute__ ((weak, alias("default_handler")));
void debugmon_handler  (void) __attribute__ ((weak, alias("default_handler")));
void pendsv_handler    (void) __attribute__ ((weak, alias("default_handler")));
void systick_handler   (void) __attribute__ ((weak, alias("default_handler")));

int main(void);

// características da SRAM do ARM Cortex-M
#define SRAM_START 0X20000000U
#define SRAM_SIZE (128U * 1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)

// definindo início da stack no topo da SRAM
#define STACK_START SRAM_END

// vetor de interrupções
// na posição 0 guarda o início do stack pointer
uint32_t vectors[] __attribute__((section(".isr_vectors"))) =
{
  STACK_START,                            /* 0x0000 0000 */
  (uint32_t)reset_handler,                /* 0x0000 0004 */
  (uint32_t)nmi_handler,                  /* 0x0000 0008 */
  (uint32_t)hardfault_handler,            /* 0x0000 000c */
  (uint32_t)memmanage_handler,            /* 0x0000 0010 */
  (uint32_t)busfault_handler,             /* 0x0000 0014 */
  (uint32_t)usagefault_handler,           /* 0x0000 0018 */
  0,                                      /* 0x0000 001c */
  0,                                      /* 0x0000 0020 */
  0,                                      /* 0x0000 0024 */
  0,                                      /* 0x0000 0028 */
  (uint32_t)svc_handler,                  /* 0x0000 002c */
  (uint32_t)debugmon_handler,             /* 0x0000 0030 */
  0,                                      /* 0x0000 0034 */
  (uint32_t)pendsv_handler,               /* 0x0000 0038 */
  (uint32_t)systick_handler,              /* 0x0000 003c */
};

// variáveis exportadas pelo linker script
extern uint32_t _sdata; // inicio da .data -SRAM
extern uint32_t _edata; // fim da .data -FLASH
extern uint32_t _la_data; //endereço onde carregar a .data na RAM

extern uint32_t _sbss; // inicio da .bss
extern uint32_t _ebss; // fim da .bss


// rotina para tratamento da reset exception
// inicializa .data, copia para SRAM
// preenche .bss com 0
// chama main()
void reset_handler(void) {

    uint32_t i;

    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata; //calcula tamanho
    uint8_t *pDst = (uint8_t*)&_sdata; //ponteiro com início da .data
    uint8_t *pSrc = (uint8_t*)&_la_data; //ponteiro com início da origem da .data na flash

    for(i=0;i<size;i++) {
        // copia para ram conteúdo de cada endereço da flash
        *pDst++ = *pSrc++; //depois de copiar soma 1 nas duas
    };

    size = (uint32_t)&_ebss - (uint32_t)&_sbss; //calcula tamanho
    pDst = (uint8_t*)&_sbss; //ponteiro com início da .bss

    for (i = 0; i < size; i++)
    {
        // coloca 0 na .bss inteira
        *pDst++ = 0;
    }

    main();
    

}

void default_handler(void) {
    while (1){};
}