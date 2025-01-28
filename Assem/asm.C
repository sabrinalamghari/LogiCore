#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour convertir une chaîne binaire en hexadécimal
void traduireEnHexadecimal(const char* binaire, char* hexa) {
    const char* dictionnaire = "0123456789abcdef";
    if (strlen(binaire) != 16) {
        fprintf(stderr, "Erreur : longueur binaire incorrecte\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 4; i++) {
        int valeur = 0;
        for (int j = 0; j < 4; j++) {
            valeur = (valeur << 1) | (binaire[i * 4 + j] - '0');
        }
        hexa[i] = dictionnaire[valeur];
    }
    hexa[4] = '\0';
}

void encoderValeur(int valeur, int taille, char* binaire, int position) {
    for (int i = position + taille - 1; i >= position; i--) {
        binaire[i] = (valeur % 2) + '0';
        valeur /= 2;
    }
}

// Encoder `movs Rd, #imm`
void encoderMOVS(const char* registre, int valeur, char* binaire) {
    int reg = atoi(registre + 1); // Extraire le numéro du registre
    printf("Encodage MOVS : registre=%s, valeur=%d\n", registre, valeur);
    if (reg < 0 || reg > 7 || valeur < 0 || valeur > 255) {
        fprintf(stderr, "Erreur : registre ou valeur non valide pour MOVS\n");
        strcpy(binaire, "");
        return;
    }
    for (int i = 15; i >= 0; i--) {
        if (i >= 8) { // Valeur immédiate (8 bits)
            binaire[i] = (valeur % 2) + '0';
            valeur /= 2;
        } else if (i >= 5) { // Registre (3 bits)
            binaire[i] = (reg % 2) + '0';
            reg /= 2;
        } else { // Opcode (5 bits)
            binaire[i] = "00100"[4 - i];
        }
    }
    binaire[16] = '\0';
    printf("Binaire généré pour MOVS : %s\n", binaire);
}

// Encoder `lsls Rd, Rs, #imm`
void encoderLSLS(const char* regDest, const char* regSrc, int imm, char* binaire) {
    int rd = atoi(regDest + 1);
    int rs = atoi(regSrc + 1);
    if (rd < 0 || rd > 7 || rs < 0 || rs > 7 || imm < 0 || imm > 31) {
        fprintf(stderr, "Erreur : paramètres incorrects pour LSLS\n");
        strcpy(binaire, "");
        return;
    }
    memset(binaire, '0', 16);
    encoderValeur(0b00000, 5, binaire, 0);  // Opcode
    encoderValeur(rs, 3, binaire, 5);      // Rs
    encoderValeur(rd, 3, binaire, 8);      // Rd
    encoderValeur(imm, 5, binaire, 11);    // Immédiat
    binaire[16] = '\0';
}

// Encoder `lsrs Rd, Rs, #imm`
void encoderLSRS(const char* regDest, const char* regSrc, int imm, char* binaire) {
    int rd = atoi(regDest + 1);
    int rs = atoi(regSrc + 1);
    if (rd < 0 || rd > 7 || rs < 0 || rs > 7 || imm < 0 || imm > 31) {
        fprintf(stderr, "Erreur : paramètres incorrects pour LSRS\n");
        strcpy(binaire, "");
        return;
    }
    memset(binaire, '0', 16);
    encoderValeur(0b00001, 5, binaire, 0);  // Opcode
    encoderValeur(rs, 3, binaire, 5);      // Rs
    encoderValeur(rd, 3, binaire, 8);      // Rd
    encoderValeur(imm, 5, binaire, 11);    // Immédiat
    binaire[16] = '\0';
}

// Encoder `subs Rd, Rs, #imm`
void encoderSUBS(const char* regDest, const char* regSrc, int imm, char* binaire) {
    int rd = atoi(regDest + 1);
    int rs = atoi(regSrc + 1);
    if (rd < 0 || rd > 7 || rs < 0 || rs > 7 || imm < 0 || imm > 7) {
        fprintf(stderr, "Erreur : paramètres incorrects pour SUBS\n");
        strcpy(binaire, "");
        return;
    }
    memset(binaire, '0', 16);
    encoderValeur(0b000111, 6, binaire, 0); // Opcode
    encoderValeur(rs, 3, binaire, 6);       // Rs
    encoderValeur(rd, 3, binaire, 9);       // Rd
    encoderValeur(imm, 3, binaire, 12);     // Immédiat
    binaire[16] = '\0';
}

// Encoder `asrs Rd, Rs, #imm`
void encoderASRS(const char* regDest, const char* regSrc, int imm, char* binaire) {
    int rd = regDest[1] - '0';
    int rs = regSrc[1] - '0';
    printf("Encodage ASRS : regDest=%s, regSrc=%s, imm=%d\n", regDest, regSrc, imm);
    if (rd < 0 || rd > 7 || rs < 0 || rs > 7 || imm < 0 || imm > 31) {
        fprintf(stderr, "Erreur : paramètres incorrects pour ASRS\n");
        strcpy(binaire, "");
        return;
    }
    for (int i = 15; i >= 0; i--) {
        if (i >= 11) { // Opcode
            binaire[i] = "00010"[15 - i];
        } else if (i >= 8) { // Rs
            binaire[i] = (rs % 2) + '0';
            rs /= 2;
        } else if (i >= 5) { // Rd
            binaire[i] = (rd % 2) + '0';
            rd /= 2;
        } else { // Imm
            binaire[i] = (imm % 2) + '0';
            imm /= 2;
        }
    }
    binaire[16] = '\0';
    printf("Binaire généré pour ASRS : %s\n", binaire);
}

// Encoder `adds Rd, Rs, Rt`
void encoderADDS(const char* regDest, const char* regSrc1, const char* regSrc2, char* binaire) {
    int rd = atoi(regDest + 1);
    int rs1 = atoi(regSrc1 + 1);
    int rs2 = atoi(regSrc2 + 1);
    printf("Encodage ADDS : regDest=%s, regSrc1=%s, regSrc2=%s\n", regDest, regSrc1, regSrc2);
    if (rd < 0 || rd > 7 || rs1 < 0 || rs1 > 7 || rs2 < 0 || rs2 > 7) {
        fprintf(stderr, "Erreur : paramètres incorrects pour ADDS\n");
        strcpy(binaire, "");
        return;
    }
    memset(binaire, '0', 16);
    encoderValeur(0b0001100, 7, binaire, 0); // Opcode
    encoderValeur(rs1, 3, binaire, 7);       // Rs1
    encoderValeur(rd, 3, binaire, 10);       // Rd
    encoderValeur(rs2, 3, binaire, 13);      // Rs2
    binaire[16] = '\0';
    printf("Binaire généré pour ADDS : %s\n", binaire);
}


// Traduire une instruction
void traduireInstruction(const char* ligne, char* binaire) {
    char trimmed[256];
    sscanf(ligne, "%[^\n]", trimmed);

    // Nettoyer les espaces
    char* start = trimmed;
    while (*start == ' ') start++;
    char* end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';

    if (start[0] == '\0' || start[0] == '@') {
        strcpy(binaire, "");
        return;
    }

    printf("Instruction détectée : %s\n", start);

    if (strstr(start, "movs") != NULL) {
        char reg[3];
        int valeur;
        if (sscanf(start, "movs %2s , #%d", reg, &valeur) == 2) {
            encoderMOVS(reg, valeur, binaire);
        }
    } else if (strstr(start, "lsls") != NULL) {
        char regDest[3], regSrc[3];
        int imm;
        if (sscanf(start, "lsls %2s , %2s , #%d", regDest, regSrc, &imm) == 3) {
            encoderLSLS(regDest, regSrc, imm, binaire);
        }
    } else if (strstr(start, "lsrs") != NULL) {
        char regDest[3], regSrc[3];
        int imm;
        if (sscanf(start, "lsrs %2s , %2s , #%d", regDest, regSrc, &imm) == 3) {
            encoderLSRS(regDest, regSrc, imm, binaire);
        }
    } else if (strstr(start, "subs") != NULL) {
        char regDest[3], regSrc[3];
        int imm;
        if (sscanf(start, "subs %2s , %2s , #%d", regDest, regSrc, &imm) == 3) {
            encoderSUBS(regDest, regSrc, imm, binaire);
        }
    } else if (strstr(start, "asrs") != NULL) {
        char regDest[3], regSrc[3];
        int imm;
        if (sscanf(start, "asrs %2s , %2s , #%d", regDest, regSrc, &imm) == 3) {
            encoderASRS(regDest, regSrc, imm, binaire);
        }
    } else if (strstr(start, "adds") != NULL) {
        char regDest[3], regSrc1[3], regSrc2[3];
        if (sscanf(start, "adds %2s , %2s , %2s", regDest, regSrc1, regSrc2) == 3) {
            encoderADDS(regDest, regSrc1, regSrc2, binaire);
        }
    } else {
        fprintf(stderr, "Erreur : instruction inconnue - %s\n", start);
        strcpy(binaire, "");
    }
}

// Programme principal
int main() {
    char sourcePath[256], ciblePath[256];
    printf("Chemin source : ");
    scanf("%255s", sourcePath);
    printf("Chemin cible : ");
    scanf("%255s", ciblePath);

    FILE* sourceFile = fopen(sourcePath, "r");
    if (!sourceFile) {
        perror("Erreur d'ouverture du fichier source");
        return EXIT_FAILURE;
    }

    FILE* cibleFile = fopen(ciblePath, "w");
    if (!cibleFile) {
        perror("Erreur d'ouverture du fichier cible");
        fclose(sourceFile);
        return EXIT_FAILURE;
    }

    fprintf(cibleFile, "v2.0 raw\n");
    char line[256];
    while (fgets(line, sizeof(line), sourceFile)) {
        char binaire[17] = {0};
        char hexa[5] = {0};

        traduireInstruction(line, binaire);

        if (strlen(binaire) == 16) {
            traduireEnHexadecimal(binaire, hexa);
            fprintf(cibleFile, "%s ", hexa);
            printf("Hexa écrit : %s\n", hexa);
        } else if (strlen(binaire) != 0) {
            fprintf(stderr, "Erreur : Longueur binaire incorrecte pour : %s\n", line);
        }
    }

    fclose(sourceFile);
    fclose(cibleFile);
    printf("SUCCESS\n");
    return EXIT_SUCCESS;
}
