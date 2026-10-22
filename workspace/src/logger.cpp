#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define TUBO "tuberia_logger"

int main(int argc, char **argv) {
    int fd;
    char buffer[1024];
    int bytes_leidos;

    // borrar FIFO por si existiera previamente
    unlink(TUBO);

    /* crear el FIFO */
    if (mkfifo(TUBO, 0666)<0) {
        perror("No puede crearse el FIFO");
        return(1);
    }
    
    /* Abrir el FIFO */
    fd = open(TUBO, O_RDONLY);
    if (fd < 0) {
        perror("No puede abrirse el FIFO");
        return(1);
    }
    /* Leer de la FIFO */
    while ((bytes_leidos = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
    
    	buffer[bytes_leidos] = '\0';
    	
	// X5: Imprimir el texto en la pantalla
	printf("%s\n", buffer);
	
    }
    close(fd);
    
    unlink(TUBO);
    return(0);
}
