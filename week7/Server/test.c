FILE *f1;
        f1 = fopen("account.txt","r");
        scanlist(f1);
        fclose(f1);
        root=reverse(root);
        updatefile2(root);
        int server_fd, connfd,recv_len,PORT; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address);
        PORT = atoi(argv[1]);
        fd_set readfds;
        fd_set masterfds; // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
        FD_ZERO(&masterfds);
        FD_ZERO(&readfds);
        FD_SET(server_fd,&masterfds); // add serverSock vào tập masterfds.
        int max_fd = server_fd; 
        struct timeval timeout;
        timeout.tv_sec = 90; // Server sẽ lắng nghe trong 90s, nếu tham số timeout = NULL thì select sẽ chạy mãi.
        timeout.tv_usec = 0;
        int n_select;
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
            perror("socket failed"); 
            exit(EXIT_FAILURE); 
        }  
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
        { 
            perror("setsockopt"); 
            exit(EXIT_FAILURE); 
        } 
        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( PORT );
        function_bind(server_fd,address);
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        }
        while(1){
            memcpy(&readfds,&masterfds,sizeof(masterfds)); // Copy masterfds vao readfds để đợi sự kiện
            n_select = select(max_fd + 1, &readfds,NULL,NULL,&timeout); 
            // Hàm này sẽ block chương trình đến khi có 1 sự kiên ready to read xảy ra
            if (n_select < 0)
            {
                perror("SELECT");
                exit(0);
            }
            else{
                int i=1;
                if (n_select == 0)
                {
                    printf("Time out\n");
                }
                else{
                    for (i = 0; i <= max_fd;i++) // Duyệt tất cả các socket đến max_fd.
                    {
                    if (FD_ISSET(i,&readfds)) // Nếu serverSock có sự kiện ready to read nghĩa là nó có kết nối mới.
                    {
                    int close_fd = 0; // Kiểm tra xem socket có nên được đóng sau khi xử lý không
                        if (i == server_fd)
                        {                          
                            connfd = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen); // Chấp nhận kết nối đó
                            printf("New connection \n");

                            FD_SET(connfd,&masterfds); // Thêm vào masterfds set để check sự kiện.
                            if (connfd > max_fd ) max_fd = connfd;
                        }
                        else
                        {
                            checkAccount(connfd,address,root);
                        }
                        if (close_fd == 1) 
                        {
                         FD_CLR(i,&masterfds);
                         close(i);
                        }
                    }
                }
                close(connfd);
            }
        }
        }