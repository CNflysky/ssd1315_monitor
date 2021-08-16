#include "opts.h"
void parse_options(int argc, char **argv) {
  int opt = 0;
  int opti = 0;
  if (argc == 1) print_help(argv[0]);
  struct option opts[] = {{"dc", required_argument, NULL, 'd'},
                          {"reset", required_argument, NULL, 'r'},
                          {"interface", required_argument, NULL, 'i'},
                          {"spidev", required_argument, NULL, 's'},
                          {"gpiochip", required_argument, NULL, 'g'},
                          {"display-duration", required_argument, NULL, 'p'},
                          {"speed", required_argument, NULL, 'f'},
                          {"config", required_argument, NULL, 'c'},
                          {"background", no_argument, NULL, 'b'},
                          {"help", no_argument, NULL, 'h'},
                          {"version", no_argument, NULL, 'v'},
                          {"kill", no_argument, NULL, 'k'},
                          {0, 0, 0, 0}

  };
  while ((opt = getopt_long(argc, argv, "d:r:i:s:g:p:f:c:bgvk", opts, &opti)) >
         0) {
    switch (opt) {
      case 'd':
        dc = atoi(optarg);
        break;
      case 'r':
        reset = atoi(optarg);
        break;
      case 'i':
        strncpy(interface, optarg, sizeof interface);
        break;
      case 's':
        strncpy(spidev, optarg, sizeof spidev);
        break;
      case 'g':
        strncpy(gpiochip, optarg, sizeof gpiochip);
        break;
      case 'p':
        display_duration = atoi(optarg);
        break;
      case 'f':
        speed = atol(optarg);
        break;
      case 'c':
        strncpy(config_path, optarg, sizeof config_path);
        use_config = true;
        break;
      case 'b':
        background = true;
        break;
      case 'v':
        printf("version:0.1\n");
        exit(EXIT_SUCCESS);
        break;
      case 'k':
        kill_process();
        break;
      case 'h':
      default:
        print_help(argv[0]);
        break;
    }
  }
}

void print_help(const char *name) {
  printf(
      i18n("Usage:%s [OPTION]... OR\n"
           "      %s -c [CONFIG_PATH]\n"
           "-d,--dc                    specify dc pin.\n"
           "-r,--reset                 specify reset pin.\n"
           "-i,--interface             specify network interface. e.g. eth0\n"
           "-s,--spidev                specify spi bus. e.g. /dev/spidev0.0\n"
           "-g,--gpiochip              specify gpiochip. e.g. gpiochip0\n"
           "-p,--display-duration      specify display_duration.\n"
           "-f,--speed                 specify spi speed.\n"
           "-c,--config                specify config file path.\n"
           "-b,--background            runs at background.\n"
           "-k,--kill                  kill a running background instance of "
           "this "
           "program.\n"
           "-h,--help                  print this message.\n"
           "-v,--version               print version.\n"
           "\n"
           "e.g. %s -d 18 -r 17 -i eth0 -s /dev/spidev0.0 -g gpiochip0 "
           "-p 5 "
           "-f 125000000 \n"),
      name, name,name);
  exit(EXIT_SUCCESS);
}

void kill_process() {
  int pid = 0;
  FILE *fp = fopen("/run/ssd1315.pid", "r");
  if (!fp) {
    perror(i18n("open pid file failed"));
    exit(1);
  }
  fscanf(fp, "%d\n", &pid);
  fclose(fp);
  remove("/run/ssd1315.pid");
  if (kill(pid, SIGTERM) < 0) {
    perror(i18n("kill process failed"));
    exit(1);
  } else
    exit(0);
}
