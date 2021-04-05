/*
 * mdnsService.cpp
 *
 *  Created on: 6/04/2021
 *      Author: lance
 */

#include "mdnsService.h"

static const char * if_str[] = {"STA", "AP", "ETH", "MAX"};
static const char * ip_protocol_str[] = {"V4", "V6", "MAX"};

//mdnsService::mdnsService() {
//	// TODO Auto-generated constructor stub
//
//}
//
//// Unused
//void mdnsService::resolve_mdns_host(const char * host_name)
//{
//    printf("Query A: %s.local", host_name);
//    esp_ip4_addr_t addr;
//    addr.addr = 0;
//
//    esp_err_t err = mdns_query_a(host_name, 3000,  &addr);
//    if(err){
//    	mdns_free();
//    	vTaskDelay(1000/portTICK_RATE_MS);
//    	start_mdns_service();
//
//    	if(err == ESP_ERR_NOT_FOUND){
//            printf(" Host was not found!\n");
//            return;
//        }
//        printf("Query Failed");
//        return;
//    }
//
//    printf(IPSTR, IP2STR(&addr));
//}
//
//
//void find_mdns_service(const char * service_name, const char * proto)
//{
//    ESP_LOGI(TAG, "Query PTR: %s.%s.local", service_name, proto);
//
//    mdns_result_t * results = NULL;
//    esp_err_t err = mdns_query_ptr(service_name, proto, 3000, 20,  &results);
//    if(err){
//        ESP_LOGE(TAG, "Query Failed");
//        return;
//    }
//    if(!results){
//        ESP_LOGW(TAG, "No results found!");
//        return;
//    }
//
//    mdns_print_results(results);
//    mdns_query_results_free(results);
//}
//
//void mdns_print_results(mdns_result_t * results){
//    mdns_result_t * r = results;
//    mdns_ip_addr_t * a = NULL;
//    int i = 1, t;
//    while(r){
//        printf("%d: Interface: %s, Type: %s\n", i++, if_str[r->tcpip_if], ip_protocol_str[r->ip_protocol]);
//        if(r->instance_name){
//            printf("  PTR : %s\n", r->instance_name);
//        }
//        if(r->hostname){
//            printf("  SRV : %s.local:%u\n", r->hostname, r->port);
//        }
//        if(r->txt_count){
//            printf("  TXT : [%u] ", r->txt_count);
//            for(t=0; t<r->txt_count; t++){
//                printf("%s=%s; ", r->txt[t].key, r->txt[t].value);
//            }
//            printf("\n");
//        }
//        a = r->addr;
//        while(a){
//            if(a->addr.type == MDNS_IP_PROTOCOL_V6){
//                printf("  AAAA: " IPV6STR "\n", IPV62STR(a->addr.u_addr.ip6));
//            } else {
//                printf("  A   : " IPSTR "\n", IP2STR(&(a->addr.u_addr.ip4)));
//            }
//            a = a->next;
//        }
//        r = r->next;
//    }
//
//}
