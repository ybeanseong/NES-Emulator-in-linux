#include "cartridge.h"

#include <stdio.h>
#include <stdlib.h>

int cartridge_load(Cartridge *cart, const char *filename)
{
    FILE *fp;
    uint8_t header[16];

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("Failed to open ROM: %s\n", filename);
        return -1;
    }

    if (fread(header, 1, 16, fp) != 16)
    {
        printf("Failed to read iNES header\n");
        fclose(fp);
        return -1;
    }

    /* iNES magic number check */
    if (header[0] != 'N' ||
        header[1] != 'E' ||
        header[2] != 'S' ||
        header[3] != 0x1A)
    {
        printf("Invalid NES ROM\n");
        fclose(fp);
        return -1;
    }

    cart->prg_rom_size = header[4] * 16 * 1024;
    cart->chr_rom_size = header[5] * 8 * 1024;

    cart->mapper =
        (header[6] >> 4) |
        (header[7] & 0xF0);

    cart->mirroring = header[6] & 0x01;

    cart->prg_rom = malloc(cart->prg_rom_size);

    if (cart->prg_rom == NULL)
    {
        printf("Failed to allocate PRG ROM memory\n");
        fclose(fp);
        return -1;
    }

    if (fread(cart->prg_rom,
              1,
              cart->prg_rom_size,
              fp) != cart->prg_rom_size)
    {
        printf("Failed to read PRG ROM\n");
        free(cart->prg_rom);
        fclose(fp);
        return -1;
    }

    if (cart->chr_rom_size > 0)
    {
        cart->chr_rom = malloc(cart->chr_rom_size);

        if (cart->chr_rom == NULL)
        {
            printf("Failed to allocate CHR ROM memory\n");
            free(cart->prg_rom);
            fclose(fp);
            return -1;
        }

        if (fread(cart->chr_rom,
                  1,
                  cart->chr_rom_size,
                  fp) != cart->chr_rom_size)
        {
            printf("Failed to read CHR ROM\n");

            free(cart->chr_rom);
            free(cart->prg_rom);

            fclose(fp);

            return -1;
        }
    }
    else
    {
        cart->chr_rom = NULL;
    }

    fclose(fp);

    return 0;
}

void cartridge_free(Cartridge *cart)
{
    free(cart->prg_rom);
    free(cart->chr_rom);

    cart->prg_rom = NULL;
    cart->chr_rom = NULL;
}
