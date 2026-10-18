# OS_T2_V18 — 2-topshiriq (18-variant)

Fan: Operatsion tizimlar. Mavzu: pointerlar va dinamik xotira (malloc/realloc/free).
Talaba: Ravshanov Rustamjon (SRM001-3 guruh), ustoz: Bekimmetova Durdona.

**Topshiriq:** 100+ xonali ikkita katta sonni satr sifatida qabul qilib,
yig'indisini yangi dinamik satrda qaytarish.

## Fayllar

| Fayl | Mazmuni |
|------|---------|
| `main.c` | manba kodi |
| `kirish.txt` | tayyor test kiritishi (100+ xonali sonlar va xato kiritishlar) |
| `tests.txt` | test natijalari |
| `valgrind.txt` | `valgrind --leak-check=full` natijasi (xotira oqishi yo'q) |

## Kompilyatsiya va ishga tushirish

```bash
gcc -Wall -Wextra -g -o main main.c
./main                                   # qo'lda kiritish
./main < kirish.txt                      # tayyor kiritish bilan
valgrind --leak-check=full ./main < kirish.txt
```

Dastur ikkita son so'raydi va yig'indisini chiqaradi; `q` kiritilsa tugaydi.
Faqat raqamlardan iborat, manfiy bo'lmagan sonlar qabul qilinadi
(harf, minus belgisi, bo'sh satr rad etiladi). Sonning uzunligi cheklanmagan:
satr `realloc` bilan o'sib boradi.

## Xotira bilan ishlash

- `oqi_satr(char **satr)` — qatorni `malloc` + `realloc` bilan o'qiydi, natijani pointer orqali qaytaradi.
- `katta_qosh(a, b)` — yig'indini yangi `malloc` qilingan satrda qaytaradi.
- Har bir `malloc`/`realloc` natijasi `NULL` ga tekshiriladi.
- Har bir son va yig'indi satri sikl oxirida `free()` qilinadi.
