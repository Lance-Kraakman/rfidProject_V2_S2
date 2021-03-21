idf_component_register(SRCS "networkProcessor.c" INCLUDE_DIRS "/." REQUIRES nvs_flash protocol_examples_common esp_https_server mdns json inputProcessing
	EMBED_TXTFILES "../../../files/certs/cacert.pem"	"../../../files/certs/prvtkey.pem")







