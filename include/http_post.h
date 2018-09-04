#ifndef __HTTP_POST__
#define __HTTP_POST__

#define PIC_SERVER_ADDR	"47.97.190.1"
#define PIC_SERVER_PORT	80
#define PIC_SERVER_URL	"test.inteink.com"
#define SERVER_PATH "/Server/interface_picture_upload.action?appid=wmj_BZ2WeU7K1iP&appsecret=MXkMcT43JsIgAwBwVJ30EbA2ovIcpfCQ&sn=%s"

#define HTTP_HEAD 	"POST %s HTTP/1.1\r\n"\
					"Host: %s\r\n"\
					"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:59.0) Gecko/20100101 Firefox/59.0\r\n"\
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"\
					"Accept-Language: en-US,en;q=0.5\r\n"\
					"Accept-Encoding: gzip, deflate\r\n"\
					"Content-Type: multipart/form-data; boundary=%s\r\n"\
					"Content-Length: %ld\r\n"\
					"Connection: close\r\n"\
					"Upgrade-Insecure-Requests: 1\r\n"\
					"DNT: 1\r\n\r\n"\
					
					
#define UPLOAD_REQUEST	"--%s\r\n"\
						"Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n"\
						"Content-Type: image/jpeg\r\n\r\n"
						
unsigned long get_file_size(const char *path);

int http_post_upload_pic(const unsigned char *IP, const unsigned int port,char *URL, const char *filepath,
									char *ack_json, int ack_len); //Post方式上传图片
		
void *Upload_Pic_Task(void *argv);

int Upload_Pic_Task_Init();


#endif
