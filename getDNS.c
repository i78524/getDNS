/******************************************************************************/
/* main進入點                                                                 */
/* Author:YiWen Chen                                                          */
/* Last update: 2020/07/29                                                    */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <math.h>

void dnsFromResolvConf(char* dns1, char* dns2) {

  char buf[128];
  int count = 0;
  FILE *pFile;
  
  pFile = fopen( "/etc/resolv.conf","r" );
  
  if( NULL == pFile ){
    printf("[dnsFromResolvConf] fopen fail, erroe 02 \n");        
  }else{

    memset(buf, 0, sizeof(buf));  
    while((fgets(buf, sizeof(buf), pFile))) {

      //printf("[myNetFromResolvConf] buf = %s", buf);
      
      // 以下為範例
      // nameserver 168.95.1.1
      // 如果資訊可用 一開頭一定要是nameserver
      if(strncmp(buf, "nameserver", 10) == 0) {
      
        int start = 0, end = 0;
        int i = 0;
        for(i = 1; i < 127; i++) {
          if((buf[i-1]=='e') && (buf[i]=='r') && (buf[i+1]==' ')) {
            start = i + 2;
          }
          
          if(buf[i] == '\n') {
            end = i - 1;
            break;
          }
        }
        
        if((end - start) > 0) {
        
          count++;          
          if(count == 1) {
            memcpy(dns1, &(buf[start]), end - start + 1);
          } else if (count == 2) {
            memcpy(dns2, &(buf[start]), end - start + 1);          
          }
          
        } else {
          printf("[dnsFromResolvConf] erroe 01 \n");        
        }
        
        if(count == 2) {
          // 最多只找兩個，因此離開
          break;
        }        
      }

    }
    
    fclose(pFile);
  }
}




int main(int argc, char **argv){

  char dns01[128];
  char dns02[128];
  memset(dns01, 0, sizeof(dns01));
  memset(dns02, 0, sizeof(dns02));
  
  dnsFromResolvConf(dns01, dns02);
  
  printf("[getDNS] dns01 = %s\n", dns01);
  printf("[getDNS] dns02 = %s\n", dns02);
}
