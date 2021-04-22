idf_component_register(SRCS "WebServerSkeleton.cpp" INCLUDE_DIRS "/." REQUIRES Wifi nvs_flash esp_https_server protocol_examples_common
	EMBED_TXTFILES "../../../files/certs/cacert.pem" "../../../files/certs/prvtkey.pem")
	








