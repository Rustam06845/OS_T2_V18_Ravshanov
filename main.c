/*
 * Muallif: Ravshanov Rustamjon, SRM001-3 guruh
 * OS_T2_V18 - 2-topshiriq, 18-variant
 * 100+ xonali ikkita katta sonni satr sifatida qabul qilib, yig'indisini
 * yangi dinamik satrda qaytaradi.
 *
 * - Satr uzunligi oldindan noma'lum: realloc bilan o'sib boruvchi bufer.
 * - Har bir malloc/realloc natijasi NULL ga tekshiriladi.
 * - Barcha xotira dastur oxirida free() bilan bo'shatiladi.
 * - Argumentni pointer orqali o'zgartiradigan funksiyalar: oqi_satr(char **),
 *   nollarni_olib_tashla(char *).
 *
 * Kompilyatsiya: gcc -Wall -Wextra -o main main.c
 * Valgrind:      valgrind --leak-check=full ./main < kirish.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOSHLANGICH_HAJM 16

/* Oqish natijalari */
#define OQISH_OK      1
#define OQISH_TUGADI  0
#define OQISH_XOTIRA -1

/*
 * Standart kirishdan bitta qatorni o'qiydi. Xotira dinamik ajratiladi va
 * kerak bo'lsa realloc bilan 2 barobar kengaytiriladi.
 * Natija *satr ga (pointer orqali) yoziladi; chaqiruvchi uni free() qiladi.
 * Qaytaradi: OQISH_OK, OQISH_TUGADI (hech narsa o'qilmadi) yoki OQISH_XOTIRA.
 */
static int oqi_satr(char **satr)
{
    size_t hajm = BOSHLANGICH_HAJM;
    size_t uzunlik = 0;
    int c;
    char *bufer = malloc(hajm);

    if (bufer == NULL)
        return OQISH_XOTIRA;

    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (uzunlik + 1 >= hajm) {              /* '\0' uchun joy ham kerak */
            size_t yangi_hajm = hajm * 2;
            char *yangi = realloc(bufer, yangi_hajm);
            if (yangi == NULL) {
                free(bufer);                     /* eski blok hali ham bizniki */
                return OQISH_XOTIRA;
            }
            bufer = yangi;
            hajm = yangi_hajm;
        }
        bufer[uzunlik++] = (char)c;
    }

    if (c == EOF && uzunlik == 0) {              /* kiritish tugadi */
        free(bufer);
        return OQISH_TUGADI;
    }

    bufer[uzunlik] = '\0';
    if (uzunlik > 0 && bufer[uzunlik - 1] == '\r')   /* Windows qator oxiri */
        bufer[uzunlik - 1] = '\0';

    *satr = bufer;
    return OQISH_OK;
}

/* Satr faqat '0'..'9' raqamlaridan iborat va bo'sh emas bo'lsa 1, aks holda 0 */
static int faqat_raqam(const char *s)
{
    const char *p = s;

    if (*p == '\0')
        return 0;
    for (; *p != '\0'; p++) {
        if (*p < '0' || *p > '9')
            return 0;
    }
    return 1;
}

/* Boshidagi ortiqcha nollarni joyida olib tashlaydi ("000123" -> "123").
 * Faqat nollardan iborat bo'lsa "0" qoladi. */
static void nollarni_olib_tashla(char *s)
{
    char *p = s;

    while (*p == '0' && *(p + 1) != '\0')
        p++;
    if (p != s)
        memmove(s, p, strlen(p) + 1);
}

/*
 * Ikki katta sonni (raqamlar satri) qo'shadi.
 * Natija yangi ajratilgan xotirada qaytariladi (chaqiruvchi free() qiladi).
 * Xotira yetmasa NULL qaytaradi.
 */
static char *katta_qosh(const char *a, const char *b)
{
    size_t la = strlen(a);
    size_t lb = strlen(b);
    size_t n = (la > lb) ? la : lb;
    size_t i;
    int qoldiq = 0;
    char *natija = malloc(n + 2);               /* n raqam + 1 ta ortiqcha + '\0' */

    if (natija == NULL)
        return NULL;

    natija[n + 1] = '\0';
    /* o'ngdan chapga qo'shamiz; natija[1..n] raqamlar, natija[0] - oxirgi o'tkazma */
    for (i = 0; i < n; i++) {
        int x = (i < la) ? a[la - 1 - i] - '0' : 0;
        int y = (i < lb) ? b[lb - 1 - i] - '0' : 0;
        int yigindi = x + y + qoldiq;
        *(natija + n - i) = (char)('0' + yigindi % 10);
        qoldiq = yigindi / 10;
    }
    *natija = (char)('0' + qoldiq);

    nollarni_olib_tashla(natija);               /* boshidagi '0' ni olib tashlaydi */
    return natija;
}

/*
 * Foydalanuvchidan to'g'ri katta son so'raydi. Noto'g'ri bo'lsa xabar berib
 * qayta so'raydi. Natija *son ga yoziladi.
 * Qaytaradi: OQISH_OK, OQISH_TUGADI ('q' yoki EOF) yoki OQISH_XOTIRA.
 */
static int son_so_ra(const char *taklif, char **son)
{
    for (;;) {
        char *satr = NULL;
        int holat;

        printf("%s", taklif);
        holat = oqi_satr(&satr);
        if (holat != OQISH_OK)
            return holat;

        if (strcmp(satr, "q") == 0 || strcmp(satr, "Q") == 0) {
            free(satr);
            return OQISH_TUGADI;
        }
        if (!faqat_raqam(satr)) {
            printf("Xato: faqat raqamlardan iborat manfiy bo'lmagan butun son kiriting "
                   "(chiqish uchun q).\n");
            free(satr);
            continue;
        }
        nollarni_olib_tashla(satr);
        *son = satr;
        return OQISH_OK;
    }
}

int main(void)
{
    printf("=== Katta sonlarni qo'shish (chiqish uchun q) ===\n");

    for (;;) {
        char *a = NULL;
        char *b = NULL;
        char *yigindi;
        int holat;

        holat = son_so_ra("\nBirinchi sonni kiriting: ", &a);
        if (holat == OQISH_XOTIRA) {
            fprintf(stderr, "Xato: xotira yetarli emas.\n");
            return 1;
        }
        if (holat == OQISH_TUGADI)
            break;

        holat = son_so_ra("Ikkinchi sonni kiriting: ", &b);
        if (holat == OQISH_XOTIRA) {
            fprintf(stderr, "Xato: xotira yetarli emas.\n");
            free(a);
            return 1;
        }
        if (holat == OQISH_TUGADI) {
            free(a);
            break;
        }

        yigindi = katta_qosh(a, b);
        if (yigindi == NULL) {
            fprintf(stderr, "Xato: xotira yetarli emas.\n");
            free(a);
            free(b);
            return 1;
        }

        printf("A (%zu xona) = %s\n", strlen(a), a);
        printf("B (%zu xona) = %s\n", strlen(b), b);
        printf("A + B (%zu xona) = %s\n", strlen(yigindi), yigindi);
        printf("[%%p] yig'indi satri heap'da joylashgan manzil: %p\n", (void *)yigindi);

        free(a);
        free(b);
        free(yigindi);
    }

    printf("\nDastur tugadi.\n");
    return 0;
}
