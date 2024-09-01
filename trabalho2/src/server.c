#include "../include/server.h"

void error(const char *msg) {
    fprintf(stderr, "%d: ", errno);
    perror(msg);
    exit(1);
}

int start_socket(int portno, int maxpending) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0) {
        error("Erro on setsockopt");
    }

    printf("Server Socket created successfully!\n");
    struct sockaddr_in serverAddr;
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        error("ERROR on binding");
    }

    if (listen(sockfd, maxpending) < 0) error("ERROR on binding");

    printf("Bind on port %d.\n", portno);

    return sockfd;
}

int accept_connection(int sockfd) {
    socklen_t clilen;
    int cli_sockfd;
    struct sockaddr_in cli_addr;
    memset(&cli_addr, '\0', sizeof(cli_addr));

    clilen = sizeof(cli_addr);
    cli_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (cli_sockfd < 0) {
        if (errno == EMFILE || errno == ENFILE) {
            perror("Errno::EMFILE: Too many open files");
            return -1;
        }
        error("ERROR on accept");
    }

    return cli_sockfd;
}

void connection_handler(int cli_sockfd) {
    char buffer[BUFFSIZE] = {0};
    int res;

    printf("waiting for request...\n");

    res = read(cli_sockfd, buffer, BUFFSIZE);

    if (res < 0) {
        error("ERROR on read");
    } else if (res == 0) {
        printf("Connection closed by client.\n");
        close(cli_sockfd);
        return;
    }

    request_handler(cli_sockfd, buffer);

    close(cli_sockfd);
}

void send_response(int cli_sockfd, const char *code, const char *type,
                   const char *content, size_t content_len) {
    char header[BUFFSIZE];
    snprintf(header, BUFFSIZE,
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s; charset=UTF-8\r\n"
             "Content-Length: %zu\r\n"
             "\r\n",
             code, type, content_len);

    if (write(cli_sockfd, header, strlen(header)) < 0) {
        close(cli_sockfd);
        error("ERROR on sending header!");
    }
    if (write(cli_sockfd, content, content_len) < 0) {
        close(cli_sockfd);
        error("ERROR on sending content!");
    }
}

char *return_mime(const char *path) {
    const char *extension = strrchr(path, '.');

    if (extension != NULL) {
        extension++;

        if (strcmp(extension, "html") == 0) {
            return "text/html";
        }

        else if (strcmp(extension, "css") == 0) {
            return "text/css";
        }

        else if (strcmp(extension, "webp") == 0) {
            return "image/webp";
        }

        else if (strcmp(extension, "jpeg") == 0) {
            return "image/jpeg";
        }

        else if (strcmp(extension, "jpg") == 0) {
            return "image/jpg";
        }

        else if (strcmp(extension, "png") == 0) {
            return "image/png";
        }

        else if (strcmp(extension, "pdf") == 0) {
            return "application/pdf";
        }
    }

    return "application/octet-stream";
}

void request_handler(int cli_sockfd, const char *buffer) {
    char path[MAX_PATH_LEN];
    char full_path[MAX_PATH_LEN];

    if (strncmp(buffer, "GET ", 4) == 0) {
        const char *start = buffer + 4;
        const char *end = strchr(start, ' ');
        if (end != NULL) {
            size_t length = end - start;
            strncpy(path, start, length);
            path[length] = '\0';
        } else {
            char *code = "400 Bad Request";
            char *type = "text/html";
            char *content =
                "<html>"
                "<head>"
                "<meta http-equiv=\"content-type\" content=\"text/html; "
                "charset=UTF-8\"/>"
                "<title>400 Bad Request</title>"
                "</head>"
                "<body>"
                "<h1>Bad Request</h1>"
                "<p>Your browser sent a request that this server could not "
                "understand.</p>"
                "</body>"
                "</html>";
            size_t content_len = strlen(content);
            send_response(cli_sockfd, code, type, content, content_len);
            return;
        }
    } else {
        char *code = "400 Bad Request";
        char *type = "text/html";
        char *content =
            "<html>"
            "<head>"
            "<meta http-equiv=\"content-type\" content=\"text/html; "
            "charset=UTF-8\"/>"
            "<title>400 Bad Request</title>"
            "</head>"
            "<body>"
            "<h1>Bad Request</h1>"
            "<p>Your browser sent a request that this server could not "
            "understand.</p>"
            "</body>"
            "</html>";
        size_t content_len = strlen(content);
        send_response(cli_sockfd, code, type, content, content_len);
        return;
    }

    snprintf(full_path, MAX_PATH_LEN, "./site%s", path);

    struct stat file_stat;
    off_t offset;
    ssize_t remain_data;
    ssize_t sent_bytes = 0;
    int file_d = open(full_path, O_RDONLY);
    if (file_d < 0) {
        char *code = "404 Not Found";
        char *type = "text/html";
        char *content =
            "<html>"
            "<head>"
            "<meta http-equiv=\"content-type\" content=\"text/html; "
            "charset=UTF-8\"/>"
            "<title>404 Not Found</title>"
            "</head>"
            "<body>"
            "<h1>Not Found</h1>"
            "<p>The requested URL was not found on this server.</p>"
            "</body>"
            "</html>";
        size_t content_len = strlen(content);
        send_response(cli_sockfd, code, type, content, content_len);
        return;
    }

    if (fstat(file_d, &file_stat) < 0) {
        char *code = "500 Internal Server Error";
        char *type = "text/html";
        char *content =
            "<html>"
            "<head>"
            "<meta http-equiv=\"content-type\" content=\"text/html; "
            "charset=UTF-8\"/>"
            "<title>500 Internal Server Error</title>"
            "</head>"
            "<body>"
            "<h1>Internal Server Error</h1>"
            "<p>The server encountered an unexpected condition that prevented "
            "it from fulfilling the request.</p>"
            "</body>"
            "</html>";
        size_t content_len = strlen(content);
        send_response(cli_sockfd, code, type, content, content_len);
        close(file_d);
        close(cli_sockfd);
        error("ERROR on fstat");
    }

    char *type = return_mime(full_path);
    char *code = "200 OK";

    char header[BUFFSIZE];
    snprintf(header, BUFFSIZE,
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s; charset=UTF-8\r\n"
             "Content-Length: %zu\r\n"
             "\r\n",
             code, type, file_stat.st_size);

    if (write(cli_sockfd, header, strlen(header)) < 0) {
        close(cli_sockfd);
        error("ERROR on sending header!");
    }

    offset = 0;
    remain_data = file_stat.st_size;
    while (
        ((sent_bytes = sendfile(cli_sockfd, file_d, &offset, BUFFSIZE)) > 0) &&
        (remain_data > 0)) {
        remain_data -= sent_bytes;
        printf("Sent: %zd bytes, Remaining: %zd bytes, Offset: %zd\n",
               sent_bytes, remain_data, offset);
    }

    if (sent_bytes == -1) {
        perror("sendfile");
    }

    close(file_d);
}