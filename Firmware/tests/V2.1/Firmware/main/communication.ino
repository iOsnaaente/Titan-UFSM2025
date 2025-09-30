// /*  
//  *  Description: Protocolo de comunicação Titan Ver:2.1.2
//  *  Author: Bruno G. F. Sampaio
//  *  Date: 07/03/2024
//  *  License: MIT
//  */

// #include "freertos/event_groups.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/semphr.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_event.h"
// #include "esp_wifi.h"
// #include "esp_log.h"

// #include "lwip/sockets.h"
// #include "lwip/netdb.h"
// #include "lwip/err.h"
// #include "lwip/sys.h"
// #include "lwip/api.h"

// #include "nvs_flash.h"

// #include "pb_common.h"
// #include "pb_decode.h"
// #include "pb_encode.h"
// #include "pb.h"


// // Para tentar reconectar caso perca a conexão
// volatile uint8_t wifi_retry_conn = 0;

// // Indica o status da conexão wifi
// volatile bool wifi_connection_state = false;

// // IP e MAC
// esp_ip4_addr_t IP;
// uint8_t mac_addr[6];


// // Eventos WiFi
// static void communication_handler(void *event_handler_arg, const char *event_base, int32_t event_id, void *event_data) {
//   // Quando o STA é conectado
//   if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
//     wifi_event_ap_staconnected_t *sta_connected = (wifi_event_ap_staconnected_t *)event_data;
//     wifi_connection_state = true;
//     wifi_retry_conn = 0;
//     // Quando o STA é desconectado
//   } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
//     wifi_event_ap_stadisconnected_t *sta_disconnected = (wifi_event_ap_stadisconnected_t *)event_data;
//     wifi_connection_state = false;
//     if ((wifi_retry_conn++) != 10) {
//       esp_wifi_connect();
//       #ifdef DEBUG_UART 
//         printf("Try reconnect %d times... \n", wifi_retry_conn);
//       #endif /* DEBUG_UART */ 
//     } else {
//       esp_restart();
//       #ifdef DEBUG_UART 
//         printf("Rebooting\n");
//       #endif /* DEBUG_UART */ 
//     }
//     // Quando o VSSS recebe um IP do mestre
//   } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//     ip_event_got_ip_t *sta_got_ip = (ip_event_got_ip_t *)event_data;
//     IP = sta_got_ip->ip_info.ip;
//     esp_wifi_get_mac(WIFI_IF_STA, mac_addr);
//     #ifdef DEBUG_UART 
//       // Imprime o MAC do dispositivo e o IP recebido do mestre
//       printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//       printf("IP: %s\n", ip4addr_ntoa((const ip4_addr_t *)&(sta_got_ip->ip_info.ip)));
//     #endif /* DEBUG_UART */ 
//     wifi_connection_state = true;
//   }
// }


// void communication_task(void *pvParameters) {
//   char rx_buffer[VSS_ROBOT_COMMANDS_PB_H_MAX_SIZE];
//   char addr_str[128];
//   int addr_family;
//   int ip_protocol;

//   // Inicia o non-volatile storage para armazenamento das variáveis do wifi
//   nvs_flash_init();
//   // Inicia as configurações padrão para inicialização de uma rede TCP/IP
//   esp_netif_init();
//   esp_event_loop_create_default();
//   // Cria e configura a interface WiFi no modo AP_STA
//   esp_netif_create_default_wifi_sta();
//   // esp_netif_create_default_wifi_ap();
//   wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
//   esp_wifi_init(&wifi_initiation);
//   // Cria os handlers de interrupções wifi
//   esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &communication_handler, NULL, NULL);
//   esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &communication_handler, NULL, NULL);
//   // Seta as configurações da conexão
//   wifi_config_t wifi_config;
//   memset(&wifi_config, 0, sizeof(wifi_config));
//   strcpy((char *)wifi_config.sta.ssid, SSID_MASTER);
//   strcpy((char *)wifi_config.sta.password, PSD_MASTER);
//   esp_wifi_set_config((wifi_interface_t)ESP_IF_WIFI_STA, &wifi_config);
//   esp_wifi_start();
//   // Seleciona o modo STA
//   esp_wifi_set_mode(WIFI_MODE_STA);
//   // Aguarde a conexão WiFi
//   esp_wifi_connect();
//   while (!wifi_connection_state) {
//   };
//   #ifdef DEBUG_UART
//     printf("Conectado ao WiFi %s!!\n", SSID_MASTER);
//     printf("Iniciando comunicação UDP/IP.\nEscutando em (%s:%d)\n", ip4addr_ntoa((const ip4_addr_t *)&IP), LISTEN_PORT);
//   #endif /* DEBUG_UART */ 

//   // Iniciando o Loop da task 
//   while (true) {
//     // Configura a conexão para o modo UDP
//     struct sockaddr_in dest_addr;
//     dest_addr.sin_family = AF_INET;
//     dest_addr.sin_port = htons(LISTEN_PORT);
//     dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     addr_family = AF_INET;
//     ip_protocol = IPPROTO_UDP;
//     inet_ntoa_r(((struct sockaddr_in6 *)&dest_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);

//     int listen_sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
//     if (listen_sock < 0) {
//       #ifdef DEBUG_UART 
//         printf("Unable to create socket: errno %d", errno);
//       #endif /* DEBUG_UART */ 
//       break;
//     }
//     int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
//     if (err != 0) {
//       #ifdef DEBUG_UART 
//         printf("Socket unable to bind: errno %d", errno);
//       #endif /* DEBUG_UART */ 
//       break;
//     }
//     #ifdef DEBUG_UART 
//       printf("Socket bound, port %d\n", LISTEN_PORT);
//       printf("Socket listening...\n");
//     #endif /* DEBUG_UART */ 

//     struct sockaddr_in source_addr;
//     uint addr_len = sizeof(source_addr);

//     while (true) {
//       int len = recvfrom(listen_sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &addr_len);
//       // Falha no recebimento 
//       if (len < 0) {
//         #ifdef DEBUG_UART 
//           printf("recv failed: errno %d", errno);
//         #endif /* DEBUG_UART */ 
//         break;

//       // Dados recebidos
//       } else if ( len > 0 ) {
//         rx_buffer[len] = 0;
        
//         #ifdef DEBUG_UART // Recebe e envia de volta os dados como echo para o cliente 
//           printf("Received %d bytes: %s\n", len, rx_buffer);
//           #ifdef SEND_ECHO 
//             err = sendto(listen_sock, rx_buffer, len, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
//             if (err < 0) {
//               printf("Error sending data back to client: errno %d\n", errno);
//             }
//           #endif /* SEND_ECHO */ 
//         #endif /* DEBUG_UART */ 

//         #ifdef PROTOBUFF  // Desserealiza os bytes no formato Protobuff
//           // Cria um stream para a desserialização da mensagem 
//           VSSCommand vssCommand = VSSCommand_init_default;
//           pb_istream_t stream = pb_istream_from_buffer( (pb_byte_t *)rx_buffer, len );
//           // Desserializa os dados recebidos na instância do RobotCommand
//           if ( pb_decode( &stream, VSSCommand_fields, &vssCommand) ) {
//             #ifdef DEBUG_UART
//             printf( "Dados processados com sucesso!\n");
//             printf( "ID do Robô: %d\n", vssCommand.id );
//             printf( "Velocidade X: %d\n", vssCommand.x );
//             printf( "Velocidade Y: %d\n", vssCommand.y );
//             printf( "Velocidade Theta: %d\n", vssCommand.theta );
//             printf( "Velocidade Orientation: %d\n", vssCommand.orientation );
//             #endif  /* DEBUG_UART */ 
//             // Armazenar os dados recebidos via Protobuff em variável global 
//             if (xSemaphoreTake(muxComm, portMAX_DELAY) == pdTRUE) {
//               // ARMAZENAR AQUI NOS VALORES QUE QUISER 
//               _r_ref = vssCommand.x;
//               _l_ref = vssCommand.y;
//               // Libera o semafaro 
//               xSemaphoreGive(muxComm);
//             }

//           // Finalizando o recebimento do pacote protobuff 
//           }else{
//             #ifdef DEBUG_UART
//               printf("Erro ao desserializar o comando do robô.\n");
//             #endif /* DEBUG_UART */ 
//             return;
//           }
//         #endif /* PROTOBUFF */ 
//       }
//     }
//   }
//   vTaskDelete(NULL);
// }
