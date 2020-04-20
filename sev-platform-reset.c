#include <linux/kvm.h>
#include <linux/psp-sev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

struct platform_reset {
	/* PLATFORM_RESET does not require an additional data buffer */
};

int main(void)
{
        int ret = 0;

        FILE *dev = fopen("/dev/sev", "r+");
        if (!dev) {
                perror("failed to open device special file");
                exit(1);
        }

        unsigned int sev_fd = fileno(dev);

        struct platform_reset reset;
        struct kvm_sev_cmd cmd = {
                .id = 0, /* PLATFORM_RESET's command ID is 0 */
                .data = (__u64)&reset,
                .error = 0,
                .sev_fd = sev_fd,
        };

        ret = ioctl(sev_fd, SEV_ISSUE_CMD, &cmd);
        if (ret != 0) {
                perror("platform_reset ioctl failed");
        }

        fclose(dev);
        return ret;
}
