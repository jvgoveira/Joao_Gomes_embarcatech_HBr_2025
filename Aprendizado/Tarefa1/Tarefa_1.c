/*! ============================================================================
 *
 *  Copyright(c) JV Gomes. All rights reserved.
 *
 *  Tarefa 1:
 *  @brief      Contador decrecente com registro de eventos por interrupção
 * 
 *  @file	    main.c
 *  @author     Joao Vitor G. de Oliveira
 *  @date	    14 Abril 2025
 *  @version    1.0
 * 
============================================================================ */

/* ============================   LIBRARIES  =============================== */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "hardware/sync.h"
#include "ssd1306.h"

/* =============================   MACROS   ================================ */
// Defin. Interface I2C para comunicação do modulo SSD1306:
#define SSD1306_I2C_BAUDRATE 400    //DEfine a freq do canal como 400 KHz
#define SSD1306_I2C_PORT i2c1       //Define a porta de comunicação I2C
#define SSD1306_I2C_SDA 14          //Define o pino 14 como SDA
#define SSD1306_I2C_SCL 15          //Define o pino 15 como SCL

// Defin. do display Oled (SSD1306):
#define OLED_ADDR 0x3C              //Define o endereco do display ole
#define OLED_WIDTH 128              //Define a largura do display
#define OLED_HEIGHT 64              //Define a altura do display

// Defin. dos pinos de acionamento dos botoes
#define BTN_A_PIN 5
#define BTN_B_PIN 6 
#define DEBOUNCE_TIME_MS 100

/* =========================   GLOBAL VARIABLES   ========================== */
// Variaveis de controle de tempo da interrupcao dos botoes
volatile uint32_t last_press_A_time = 0;
volatile uint32_t last_press_B_time = 0;

volatile bool A_pressed = 0, B_pressed = 0;
bool count_ctr = 0;

int contagem = 0;

// Registro de contagem do botão B
unsigned int contagem_B = 0;

//Registro de mensagem do display
char mensagem [10] = {};

/* ========================   FUNCTION PROTOTYPE   ========================= */

void I2c_init(void);    //Funcao de inicializacao do barramento I2c
void Gpios_init(void);   //Inicializa todos os periféricos

void gpio_callback(uint gpio, uint32_t events); // Callback da interrupcao dos botoes

//Callback Low Power Mode
bool TIM_callback(struct repeating_timer *t);

/* ===========================   MAIN FUNCTION   =========================== */
int main(void)
{
    stdio_init_all();

    I2c_init();
    Gpios_init();

    SSD1306_init();

    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area frame_area = {
        .start_col = 0,
        .end_col = SSD1306_WIDTH - 1,
        .start_page = 0,
        .end_page = SSD1306_NUM_PAGES - 1
    };

    calc_render_area_buflen(&frame_area);

    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);
    
    SSD1306_scroll(true);
    sleep_ms(1000);
    SSD1306_scroll(false);

    char *tela1[] = {
    "               ",
    " TIMMER A: OFF ",
    " CONTAGEM: 0   ",
    "               "};
    //"123456789012345"};

    // Printa a tela de inicio
    for (int i = 0, y = 0; i < count_of(tela1); i++)
    {
        WriteString(buf, 5, y, tela1[i]);
        y += 8;
    }              
    render(buf, &frame_area);
    sleep_ms(5000);

    // Configuracao de ativacao do temporizador para cada 1 segundo
    struct repeating_timer temp;
    add_repeating_timer_ms(1000, TIM_callback, NULL, &temp);

    while (true) 
    {
        if (count_ctr)
        {
            contagem_B = 0;
            contagem = 9;
            while (contagem >= 0)
            {
                sprintf(mensagem, " TIMMER A: %d s", contagem);
                WriteString(buf, 5, 15, mensagem); //Buffer , desl. horiz, desl. vert, string
                render(buf, &frame_area);

                sprintf(mensagem, " CONTAGEM: %d ", contagem_B);
                WriteString(buf, 5, 20, mensagem); //Buffer , desl. horiz, desl. vert, string
                render(buf, &frame_area); 
            }
            count_ctr = 0;
            
            sprintf(mensagem, " TIMMER A: OFF", contagem);
            WriteString(buf, 5, 15, mensagem); //Buffer , desl. horiz, desl. vert, string
            render(buf, &frame_area);
        }

        sleep_ms(10);
    }

} /* end main */

/* =======================  DEVELOPMENT OF FUNCTIONS ======================= */

/*! ---------------------------------------------------------------------------
 *  @brief      Funcao de inicializacao das GPIOs
 *  @param      none
 *  @return     Void
 * 
 ----------------------------------------------------------------------------*/
 void Gpios_init(void)
 {  
     gpio_init(BTN_A_PIN);
     gpio_set_dir(BTN_A_PIN, GPIO_IN);
     gpio_pull_up(BTN_A_PIN);
      
     gpio_init(BTN_B_PIN);
     gpio_set_dir(BTN_B_PIN, GPIO_IN);
     gpio_pull_up(BTN_B_PIN);
 
     // Configurando interrupções para os dois botões corretamente
     gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
     gpio_set_irq_enabled_with_callback(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

 }
 /*! ---------------------------------------------------------------------------
 *  @brief      Funcao de chamada de interrupcao do acionamento dos botoes
 *  @param      gpio    Numero do pino GPIO
 *  @param      evento  Evento causador da interrupcao
 *  @return     Void
 * 
 ----------------------------------------------------------------------------*/
void gpio_callback(uint gpio, uint32_t events) 
{
    uint32_t now = time_us_32();

    if (gpio == BTN_A_PIN && (now - last_press_A_time > DEBOUNCE_TIME_MS * 1000))
    {   
        count_ctr = 1;
        last_press_A_time = now;
    }
    else if (gpio == BTN_B_PIN && (now - last_press_B_time > DEBOUNCE_TIME_MS * 1000))
    {
        contagem_B ++;
        last_press_B_time = now;
    }
}
/* ===========================  I2C FUNCTIONS  ============================= */

/*! ---------------------------------------------------------------------------
 *  @brief      Funcao de inicializacao do barramento i2c
 *  @param      none
 *  @return     Void
 * 
 ----------------------------------------------------------------------------*/
 void I2c_init(void)
 {
    // Inicializacao do I2C na frequencia pre configurada para o display OLED
    i2c_init(SSD1306_I2C_PORT, SSD1306_I2C_BAUDRATE*1000);
    
    // Definicao dos pinos 
    gpio_set_function(SSD1306_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SSD1306_I2C_SDA);
    gpio_pull_up(SSD1306_I2C_SCL);
 }

 /*! ---------------------------------------------------------------------------
 *  @brief  A funcao e chamada pelo gatilho do temporizador repetidor em ciclos
 *         predefinidos de tempo
 * 
 *  @param *t ponteiro para estrutura do temporizador repetidor 
 * 
 *  @return O valor decimal referente ao valor binario de entrada
 * 
 ----------------------------------------------------------------------------*/
bool TIM_callback(struct repeating_timer *t) 
{
   contagem --;
}
 
/* end program */

