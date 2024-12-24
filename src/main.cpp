#include <curl/curl.h>
#include <SQLiteCpp/SQLiteCpp.h>
#include <spdlog/spdlog.h>
#include <wolfssl/openssl/ssl.h>


int main(int argc, char** argv) {
	CURL *curl = curl_easy_init();
	CURLcode imgresult;
	FILE *fp;
	if (curl) {
		errno_t err = fopen_s(&fp, "./google.jpg", "wb");
		curl_easy_setopt(curl, CURLOPT_URL, "https://images.squarespace-cdn.com/content/v1/539f49a3e4b0fc234463a04d/1489013554760-TXQZT540SWRPDVOW3UMM/image-asset.jpeg?format=2500w");
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		imgresult = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	fclose(fp); 
	return EXIT_SUCCESS;
}

//https://api.scryfall.com/cards/!set:uuid!?format=image&face=!prop:side!
//https://api.scryfall.com/cards/multiverse/!set:muid!?format=image
//https://gatherer.wizards.com/Handlers/Image.ashx?multiverseid=!set:muid!&type=card
//https://gatherer.wizards.com/Handlers/Image.ashx?name=!name!&type=card