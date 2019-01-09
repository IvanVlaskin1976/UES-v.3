// основная программа Универсальной Экспертной Системы
// компилировать в Linux C++ командой 
// $ c++ -o UES_v_3_03 UES_v_3_03.cpp
// запускать на исполнение командой 
// $ ./UES_v_3_03
// помощь в программе вводом команды h
#include<stdio.h>
#include<sys/stat.h>
#include <signal.h>
#include <unistd.h>     /* read(), write(), close() */
#include <stdlib.h>
#include <netinet/in.h>

#include <fcntl.h>      /* open(), O_RDONLY */


unsigned int max_chislo_iera=3999;//параметр нужный для ограничения указателя массива иерархий
unsigned int max_chislo_vetveq=99999;//параметр нужный для ограничения указателя массива ветвей
unsigned int max_dlina_iera=99999;
unsigned int max_chislo_zapiseq_iera=19998;
unsigned int max_chislo_vetveq_iera=99999;
unsigned int max_dlina_vetveq=999;
unsigned int max_chislo_zapiseq_vetvi=992;
unsigned int massiv_iera[4000][100000];
unsigned int chislo_iera=0;
unsigned int chislo_vetveq=0;
unsigned int sortirovochnaya_iera[1000][19998][7];//стандарт формата сортировочной таблицы иерархий
unsigned int massiv_vetveq[100000][1000];//стандарт формата ветви
unsigned int sort_vetveq[100000][4];//служебный массив для сортировки ветвей в каталоге каталога ветвей
unsigned int massiv_zapiseq[100000][4];//стандарт на описание ветви в иерархии, нужен для получения следующей ветви иерархии

unsigned int massiv_adress_iera[4000];// массив со ссылками на номера массива иерархий нужных для служебных целей - чтобы иметь номер иерархии каталога иерархий, каталога ветвей и прочих
char name_iera1[]="catalog_ierarhiq\0";
char name_iera2[]="catalog_vetveq\0";

unsigned int path1[100000];//первая часть имени - путь от корня до базы объектов
unsigned int path2[100000];//вторая часть имени - путь от базы объектов к объекту
unsigned int path3[100000];//третья часть имени - путь внутри объекта к опорному каталогу файла
unsigned int name1[100000];//последняя часть имени
char obschee_name_file[100000];//составное имя файла 
char obschee_name_catalog[100000];//составное имя каталога

char text1[100000];//первый рабочий массив текста, используемый для технологических нужд программы и универсально используемый разными функциями
unsigned int max_dlina_dlinnogo_text=99999;
char text2[100000];//второй рабочий массив текста, используемый для технологических нужд программы
char text3[100000];//третий рабочий массив текста, используемый для технологических нужд программы
char cifra1[3]={48,48,0};
char cifra2[3]={48,48,0};
char cifra3[3]={48,48,0};
char cifra4[3]={48,48,0};
char simvol_cataloga='/';
char simvol_tochki='.';

unsigned int tvorec=7;

unsigned int v_cataloge_iera_korn_catalog       =0x28000523;
unsigned int v_cataloge_iera_catalog            =0x28000524;
unsigned int v_cataloge_iera_dvoetochie         =0x28000525;
unsigned int v_cataloge_iera_iera               =0x28000526;

unsigned int v_cataloge_vetveq_korn_catalog     =0x28000527;
unsigned int v_cataloge_vetveq_catalog          =0x28000528;
unsigned int v_cataloge_vetveq_dvoetochie       =0x28000529;
unsigned int v_cataloge_vetveq_vetv             =0x28000530;

unsigned int v_cataloge_iera_mech_korn_catalog  =0x28000531;
unsigned int v_cataloge_iera_mech_catalog       =0x28000532;
unsigned int v_cataloge_iera_mech_dvoetochie    =0x28000533;
unsigned int v_cataloge_iera_mech_iera          =0x28000534;

unsigned int v_cataloge_vetveq_mech_korn_catalog=0x28000535;
unsigned int v_cataloge_vetveq_mech_catalog     =0x28000536;
unsigned int v_cataloge_vetveq_mech_dvoetochie  =0x28000537;
unsigned int v_cataloge_vetveq_mech_vetv        =0x28000538;

unsigned int svobodnaya_iera                    =0x28000512;
unsigned int zanyataya_iera                     =0x28000516;
unsigned int svobodnaya_vetv                    =0x28000513;
unsigned int zanyataya_vetv                     =0x28000517;

unsigned int text_banalnoe_format_koi8r        =0x28000450;
unsigned int UES_rejim_prostoq                  =0x28000601;
unsigned int UES_rejim_keyboard_buffer_full     =0x28000602;
unsigned int UES_rejim_hard_exit                =0x28000603;
unsigned int text_koi8r_page                    =0x30303000;

unsigned char massiv_text_keyboard[999999];//нужен для ввода с клавиатуры по scanf
unsigned int massiv_int_keyboard[1000000];//нужен для клавиатурного ввода в массив данных
unsigned int system_rejim1[1]={UES_rejim_prostoq};
// system_rejim1:
// UES_rejim_prostoq - простой системы
// UES_rejim_keyboard_buffer_full - заполнен клавиатурный массив
// UES_rejim_hard_exit - жесткий выход с закрытием процессов и сокитов обмена данными
int bytes_read;
int sock2, listener;
unsigned int system_int;//нужен для оперативной работы
unsigned int massiv_system_int_buffer[1000000];//нужен для оперативной работы
char massiv_system_text_buffer[999999];//нужен для оперативной работы
unsigned int monitor_command_now;//код команды монитора
char now_path[100000];//нужен для оперативной работы
char now_file[100000];//нужен для оперативной работы

unsigned int BZ_kolichestvo_virtual_iera_in_RAM=0;// количество виртуальных иерархий в ОЗУ(надо не больше 950, иначе переполнение ОЗУ)
unsigned int BZ_virtual_iera_be_iera[1000000];// соответствие виртуальной иерархии реальной иерархии
unsigned int BZ_virtual_iera_be_object[1000000];// соответствие виртуальной иерархии реальному объекту в Базе Знаний
unsigned int BZ_virtual_iera_be_name_RAM[1000][100];// соответствие виртуальной иерархии реальному имени объекта
unsigned int BZ100000_kolichestvo_massivov;//количество массивов
unsigned int BZ100000[100][100000];// массивы для харакеристик длиной 99 999 слов, 0 адрес это длина
unsigned int BZ100000_be_RAM[100];//0 - массива нет или 1 - массив занят
unsigned int max_kolichestvo_BZ100000=99;
unsigned int max_dlina_BZ100000=99999;
char BZ_path[100000]="/mnt/sda12/baza2/\0";
char BZ_spec_path[100000]="etc/\0";
char BZ_savefile1[100000]="savefile1\0";//файл сохраняющий иерархии, ветви и массивы
char BZ_file_name_object_i_nomer_object[100000]="name_object\0";//файл загружающий имена объектов и их номера
char BZ_file_name_categ_i_nomer_categ[100000]="name_categ\0";//файл загружающий имена групп и их номера
#define BUFFER_SIZE     256
#define INT_BUFFER_SIZE     1024
unsigned int buffer[BUFFER_SIZE];
unsigned int chislo_object=0;
unsigned int chislo_categ=0;
unsigned int nomer_object=1;
unsigned int nomer_categ=1;
unsigned int busy_object[20000];
unsigned int busy_categ[20000];
unsigned int name_object[20000][50];
unsigned int name_categ[20000][50];
unsigned int max_chislo_object=19999;
unsigned int max_chislo_categ=19999;

unsigned int n01;// номер служебной иерархии с установками
unsigned int n02;// номер массива BZ100000 с индексами адресов записей системной иерархии
unsigned int n03=0x1000001;// номер текущей иерархии
unsigned int n04;// номер текущей записи в текущей иерархии

//=====

// список функций в программе:
// skleivanie_imen_v_catalog1
// skleivanie_imen_v_catalog2
// skleivanie_imen_v_file1
// zapolnenie_text2
// vyvod_text
// zapolnenie1
// zapolnenie2
// zapolnenie3
// zapolnenie4
// skleivanie_text_text1_i_text2
// zapolnenie_text2_from_text1
// zapolnenie_text1_from_text2
// preobrazovanie_cifry_v_simvol1
// dozapis_simvol_k_text1
// preobrazovanie_cifry_v_psevdocatalog
// preobrazovanie_cifry1_v_text1
// preobrazovanie_cifry12_v_text1
// preobrazovanie_cifry123_v_text1
// preobrazovanie_cifry1234_v_text1
// preobrazovanie_cifry_v_catalog
// preobrazovanie_cifry1_v_text2
// preobrazovanie_cifry2_v_text2
// preobrazovanie_cifry3_v_text2
// preobrazovanie_cifry4_v_text2
// skleivanie_text_text1_i_text2_v_text3
// zapolnenie_text3_from_text2
// zapolnenie_path2_from_text2
// zapolnenie_path3_from_text2
// zapolnenie_path1_from_text2
// create_catalog1
// poisk_sleduyscheq_zapisi
// poisk_kategorii
// poisk_kategorii_start
// poisk_kategorii_i_opornogo_object
// poisk_kategorii_i_opornogo_object_start
// poisk_kategorii_i_opornogo_object_i_vtorichnogo_object
// poisk_kategorii_i_opornogo_object_i_vtorichnogo_object_start
// poisk_vtorichnogo_object
// poisk_vtorichnogo_object_start
// poisk_kategorii_i_vtorichnogo_object
// poisk_kategorii_i_vtorichnogo_object_start
// mechenie
// nahojdenie_mechenogo_object
// snyatie_mecheniya_s_object
// blijaqshaya_pustaya_iera
// blijaqshaya_pustaya_vetv
// init_vetvi
// init_vetv_massiv
// ochistka_vetvi
// perevod_iera
// udalenie_zapisi
// udalenie_vtorichnogo_object1
// udalenie_vtorichnogo_object2
// mechenie_cataloga
// poisk_v_cataloge_vetveq_object
// poisk_v_cataloge_vetveq_vetvi
// poisk_v_cataloge_vetveq_massiva
// poisk_v_iera_vetvi
// poisk_v_iera_vetvi_start
// tochnoe_vkluchenie_vetvi_v_iera
// poisk_v_cataloge_iera_vetvi
// poisk_v_cataloge_iera_harakteristik_iera
// poisk_v_cataloge_vetveq_harakteristik_vetveq
// poisk_i_mechenie_ierarhiq_cataloga_po_kategorii
// poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu
// poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu
// poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu
// poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii
// poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu
// poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu
// poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu
// snyatie_mecheniya_cataloga
// sortirovka_vetveq_v_cataloge
// perenos_mechenogo_v_catalog
// kopirovanie_mechenogo_v_catalog
// perenos_cataloga_v_catalog
// kopirovanie_cataloga_v_catalog
// izvlechenie_cataloga_v_iera
// chislo_iera_v_cataloge
// chislo_iera_i_catalogov_v_cataloge
// udalenie_mechenogo
// udalenie_catalog_iz_cataloga
// init_catalogi_iera
// init_iera
// init_iera_massiv
// ochistka_iera
// ochistka_sortirovochnaya_iera
// sortirovka_iera
// dozapis_end_iera_massiv
// dozapis_end_iera_object
// dozapis_end_iera_object_zero
// dozapis_v_catalog
// tochnoe_vklychenie_iera
// udalenie_lishnego_iera
// udalenie_host_iz_iera
// uplotnenie_cataloga_iera
// uplotnenie_cataloga_vetveq
// skleivanie_iera
// skleivanie_end_iera
// init_vetvi_iera
// next_vetv_iera

// dlina_text_to_return
// simvoly_v_text
// convert_text1
// right_text
// left_text
// mid_text
// right_rezalka_text
// left_rezalka_text
// sravnenie_text_to_return
// vkluchennost_text_to_return
// vkluchennost_right_text_to_return
// vkluchennost_left_text_to_return
// rezalka_text
// kolichestvo_text_to_return
// flag_sovpadeniya_text_to_return
// flag_sovpadeniya_right_text_to_return
// flag_sovpadeniya_left_text_to_return
// flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return
// copy_file2text
// copy_charfile2intmassiv
// copy_text2file
// copy_intmassiv2charfile
// copy_intmassiv2intfile
// copy_file2intmassiv
// copy_text2intmassiv
// copy_intmassiv2text
// copy_intmassiv1_and_intmassiv2_2intmassiv3
// copy_intmassiv1_and_intmassiv2_2intmassiv1
// copy_text1_and_text2_2text3
// copy_intmassiv1_to_intmassiv2
// copy_text1_to_text2
// ostatok_massiva_int_right
// ostatok_massiva_int_left
// ostatok_text_right
// ostatok_text_left
// convert_int1
// convert_cp1251_to_koi8r_file1_to_file2
// convert_koi8r_to_cp1251_file1_to_file2
// convert_int_utf8_to_koi8r
// convert_utf8_to_koi8r_file1_to_file2
// opredelenie_kodirovki_to_return
// find_simvola_v_massiv_to_return
// find_simvola_v_massiv_next_to_return
// find_int_v_massiv_to_return
// find_int_v_massiv_next_to_return
// convert_text_to_massiv
// convert_massiv_to_text
// find_massiv_v_massiv_to_massiv
// find_massiv_v_massiv_to_return
// find_massiv_v_massiv_next_to_return
// right_massiv
// left_massiv
// mid_massiv
// mid_massiv
// right_rezalka_massiv
// sravnenie_massiv_to_return
// vkluchennost_massiv_v_massiv_to_return
// vkluchennost_right_massiv_to_return
// rezalka_massiv_to_return
// kolichestvo_massiv_to_return
// flag_sovpadeniya_massiv_to_return
// flag_sovpadeniya_right_massiv_to_return
// ubiranie_simvola
// ubiranie_simvola_13
// dobavlenie_simvola_13
// zapolnenie_prostoq_iera_text_massivom
// unsigned int zapolnenie_massiva_int_textom_keyboard
// system_flag1
// vyvod_massiva_int
// vyvod_massiva_char
// vyvod_lf
// preobrazovanie_massiva_char_v_massiv_int
// preobrazovanie_massiva_int_v_massiv_char(unsigned int *massiv_int,
// monitor_help_info
// perevod_int_massiva_pozition_v_text
// perevod_text_v_namepath_i_namefile
// perevod_text_10cifry_v_int_to_return
// perevod_text_16cifry_v_int_to_return
// perevod_int_cifry_v_int_16rich_massiv
// save_sostoyanie_UES
// load_sostoyanie_UES
// index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol
// vyrezalka_so_start_po_dlinu
// init_i_clean_BZ100000
// vydel_1_free_BZ100000
// print_int_text_massiv
// print_16_int
// copy_unformat_text_to_UES_text
// copy_UES_text_to_unformat_text
// copy_UES_text_to_iera
// copy_from_UES_text_to_iera
// copy_from_iera_to_UES_text
// print_zapis
// print_iera
// zagruzka_zapis_int
// zagruzka_zapis_text

//=====
// main


// склеивание массивов имен в общее имя каталога obschee_name_file
// unsigned int skleivanie_imen_v_catalog1(void) {
// 110100001 = нормальный выход
// другой вариант склеивания массивов имен в общее имя  каталога obschee_name_file
// unsigned int skleivanie_imen_v_catalog2(void) {
// 110100002 = нормальный выход
// склеивание массивов имен в общее имя файла obschee_name_file
// unsigned int skleivanie_imen_v_file1(void) {
// 110100003 = нормальный выход
// заполнение технологического массива text1 другим текстовым массивом
// unsigned int zapolnenie_text1(char *x1) {
// 110100004 = нормальный выход
// 110100005 = выход за максимальную длину длинного текста
// заполнение технологического массива text2 другим текстовым массивом
// unsigned int zapolnenie_text2(char *x1) {
 // 110100006 = нормальный выход
 // 110100007 = выход за максимальную длину длинного текста
// вывод технологического массива text1 в другой текстовый массив
// unsigned int vyvod_text(char *x1) {
 // 110100008 = нормальный выход
 // 110100009 = выход за максимальную длину длинного текста
// заполнение технологическим массивом text1 массива path1 указывающего на первую часть имени
// unsigned int zapolnenie1(void) {
// 110100010 = нормальный выход
// 110100011 = выход за максимальную длину длинного текста
// заполнение технологическим массивом text1 массива path2 указывающего на вторую часть имени
// unsigned int zapolnenie2(void) {
// 110100012 = нормальный выход
// 110100013 = выход за максимальную длину длинного текста 
// заполнение технологическим массивом text1 массива path3 указывающего на третью часть имени
// unsigned int zapolnenie3(void) {
// 110100014 = нормальный выход
// 110100015 = выход за максимальную длину длинного текста
// заполнение технологическим массивом text1 массива name1 указывающего на последнюю часть имени
// unsigned int zapolnenie4(void) {
// 110100016 = нормальный выход
// 110100017 = выход за максимальную длину длинного текста
// склеивание технологических текстов text1 и text2 в text1
// unsigned int skleivanie_text_text1_i_text2(void) {
// 110100018 = выход за максимальную длину длинного текста
// 110100019 = нормальный выход
// 110100020 = выход за максимальную длину длинного текста
// заполнение технологического массива text2 другим текстовым массивом text1
// unsigned int zapolnenie_text2_from_text1(void) {
// 110100021 = нормальный выход
// 110100022 = выход за максимальную длину длинного текста
// заполнение технологического массива text1 другим текстовым массивом text2
// unsigned int zapolnenie_text1_from_text2(void) {
// 110100023 = нормальный выход
// 110100024 = выход за максимальную длину длинного текста
// преобразование цифры в символ
// char preobrazovanie_cifry_v_simvol1(int cifra) {
// дозапись символа к массиву text1
// unsigned int dozapis_simvol_k_text1(char simvol) {
// 110100025 = нормальный выход
// 110100026 = выход за максимальную длину длинного текста
// преобразование цифры в псевдокаталог cifra1 & cifra2 & cifra3 & cifra4
// unsigned int preobrazovanie_cifry_v_psevdocatalog(int cifry) {
// 110100027 = нормальный выход
// преобразование псевдокаталога cifra1 в массив-каталог text1 типа /ff
// unsigned int preobrazovanie_cifry1_v_text1(void) {
// 110100028 = нормальный выход
// преобразование псевдокаталога cifra1 & cifra2 в массив-каталог text1 типа /ff/ff
// unsigned int preobrazovanie_cifry12_v_text1(void) {
// 110100029 = нормальный выход
// преобразование псевдокаталога cifra1 & cifra2 & cifra3 в массив-каталог text1 типа /ff/ff/ff
// unsigned int preobrazovanie_cifry123_v_text1(void) {
// 110100030 = нормальный выход
// преобразование псевдокаталога cifra1 & cifra2 & cifra3 & cifra4 в массив-каталог text1 типа /ff/ff/ff/ff
// unsigned int preobrazovanie_cifry1234_v_text1(void) {
// 110100031 = нормальный выход
// преобразование цифры в каталог ff/ff/ff/ff в массиве text1
// unsigned int preobrazovanie_cifry_v_catalog(int cifry) {
// 110100032 = нормальный выход
// преобразование цифры cifra1 в каталог ff в массиве text2
// unsigned int preobrazovanie_cifry1_v_text2(void) {
// 110100033 = нормальный выход
// преобразование цифры cifra2 в каталог ff/ff в массиве text2
// unsigned int preobrazovanie_cifry2_v_text2(void) {
// 110100034 = нормальный выход
// преобразование цифры cifra3 в каталог ff/ff/ff в массиве text2
// unsigned int preobrazovanie_cifry3_v_text2(void) {
// 110100035 = нормальный выход
// 110100036 = выход за максимальную длину длинного текста
// преобразование цифры cifra4 в каталог ff/ff/ff/ff в массиве text2
// unsigned int preobrazovanie_cifry4_v_text2(void) {
// 110100037 = нормальный выход
// 110100038 = выход за максимальную длину длинного текста
// склеивание технологических текстов text1 и text2 в text3
// unsigned int skleivanie_text_text1_i_text2_v_text3(void) {
// 110100039 = выход за максимальную длину длинного текста
// 110100040 = выход за максимальную длину длинного текста
// 110100041 = нормальный выход
// заполнение технологического массива text3 другим текстовым массивом text2
// unsigned int zapolnenie_text3_from_text2(void) {
// 110100042 = нормальный выход
// 110100043 = выход за максимальную длину длинного текста
// заполнение технологического массива path2 другим текстовым массивом text2
// unsigned int zapolnenie_path2_from_text2(void) {
// 110100044 = нормальный выход
// 110100045 = выход за максимальную длину длинного текста
// заполнение технологического массива path3 другим текстовым массивом text2
// unsigned int zapolnenie_path3_from_text2(void) {
// 110100046 = нормальный выход
// 110100047 = выход за максимальную длину длинного текста
// заполнение технологического массива path1 другим текстовым массивом text3, обнуление path2,path3 и name1
// unsigned int zapolnenie_path1_from_text2(void) {
// 110100048 = нормальный выход
// 110100049 = выход за максимальную длину длинного текста
// создание директория
// unsigned int create_catalog1(void) {
// 110100050 = нормальный выход
// поиск следующей записи
// функция возвращает или адрес следующей записи иерархии или 0 если записи нет
// unsigned int poisk_sleduyscheq_zapisi(unsigned int iera, unsigned int start) {
// поиск записи по заданной категории,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii(unsigned int iera, unsigned int kategoriya) {
// поиск записи по заданной категории со стартового значения,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_start(unsigned int iera, unsigned int kategoriya,
//  unsigned int start) {
// поиск записи по заданной категории и опорному объекту,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_i_opornogo_object(unsigned int iera, 
//  unsigned int kategoriya, unsigned int object) {
// поиск записи по заданной категории и опорному объекту со стартового значения,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_i_opornogo_object_start(unsigned int iera, 
//  unsigned int kategoriya, unsigned int object, unsigned int start) {
// поиск записи по заданной категории и опорному объекту и вторичному объекту
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_i_opornogo_object_i_vtorichnogo_object(unsigned int iera, 
//  unsigned int kategoriya, unsigned int object1, unsigned int object2) {
// поиск записи по заданной категории и опорному объекту и вторичному объекту со стартового значения
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_i_opornogo_object_i_vtorichnogo_object_start(unsigned int iera, 
//  unsigned int kategoriya, unsigned int object1, unsigned int object2,
//  unsigned int start) {
// поиск записи по вторичному объекту
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_vtorichnogo_object(unsigned int iera, unsigned int object2) {
// поиск записи по вторичному объекту со стартового значения
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_vtorichnogo_object_start(unsigned int iera, unsigned int object2,
// unsigned int start) {
// поиск записи по категории и вторичному объекту
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_i_vtorichnogo_object(unsigned int iera,
//  unsigned int kategoriya, unsigned int object2) {
// поиск записи по категории и вторичному объекту со стартового значения
// функция возвращает или адрес совпадающй записи или 0 если записи нет
// unsigned int poisk_kategorii_i_vtorichnogo_object_start(unsigned int iera,
//  unsigned int kategoriya, unsigned int object2, unsigned int start) {
// мечение отдельных иерархий и отдельных ветвей
// unsigned int mechenie(unsigned int zapis) {
// 110100051 = это не иерархия
// 110100052 = это не иерархия в каталоге иерархий
// 110100053 = нормальный выход
// 110100054 = это не иерархия
// 110100055 = это не ветвь в каталоге ветвей
// 110100056 = нормальный выход
// 110100057 = это не иерархия или ветвь в каталоге иерархий или ветвей
// нахождение адреса первой меченой отдельной иерархии или отдельной ветви
// unsigned int nahojdenie_mechenogo_object(unsigned int zapis) {
// 0 = это не иерархия
// 0 = это не иерархия в каталоге иерархий
// снятие мечения с отдельных иерархий или отдельных ветвей
// unsigned int snyatie_mecheniya_s_object(unsigned int zapis) {
// 0 = это не иерархия
// 0 = это не иерархия в каталоге иерархий
// нахождение ближайшей пустой иерархии, если нет то 0
// unsigned int blijaqshaya_pustaya_iera(void) {
// нахождение ближайшей пустой ветви, если нет то 0
// unsigned int blijaqshaya_pustaya_vetv(void) {
// инициализация ветви massiv_vetveq[nomer_vetvi1]
// unsigned int init_vetvi(unsigned int nomer_vetvi1, 
//  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1,
//  unsigned int oblast1) {
// 110100058 =  это не ветвь
// 110100059 = нормальный выход
// инициализация ветви massiv_vetveq[nomer_vetvi1] массивом x1
// unsigned int init_vetv_massiv(unsigned int vetv1,
//  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1,
//  unsigned int oblast1, unsigned int *x1) {
// 110100060 = это не ветвь
// 110100428 = некорректный массив
// 110100429 = некорректный массив
// 110100061 = нормальный выход
// обнуление ветви massiv_vetveq[nomer_vetvi1] до начального значения
// unsigned int ochistka_vetvi(unsigned int nomer_vetvi1) {
// 110100062 = это не ветвь
// 110100063 = нормальный выход
// перевод важнейших параметров иерархии в сортировочную таблицу
// стандарт формата сортировочной таблицы иерархий:
// x0 = метка 1-избранная запись
// x1 = существование
// x2 = область объектов
// x3 = опорный объект
// x4 = параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// x5 = количество связей
// x6 = адрес записи в иерархии
// unsigned int perevod_iera(unsigned int nomer_iera1) {
// 110100064 = это не иерархия
// 110100065 = это пустая иерархия
// 110100066 = нормальный выход
// удаление одной записи иерархии
// unsigned int udalenie_zapisi(unsigned int iera, unsigned int adres_zapisi) {
// 110100067 = это не иерархия
// 110100068 = некорректное стартовое значение
// 110100069 = некорректный адрес записи
// 110100070 = нормальный выход
// удаление вторичного объекта из записи иерархии
// unsigned int udalenie_vtorichnogo_object1(unsigned int iera,
//  unsigned int kategoriya, unsigned int object1, unsigned int object2) {
// 110100071 = нет данного вторичного объекта
// 110100072 = это не иерархия
// 110100073 = нормальный выход
// 110100074 = нормальный выход
// удаление вторичного объекта из записи иерархии 
// unsigned int udalenie_vtorichnogo_object2(unsigned int iera, unsigned int zapis,
//  unsigned int object2) {
// 110100075 = это не иерархия
// 110100076 = некорректный адрес записи
// 110100077 = нормальный выход
// 110100078 = нормальный выход
// вложенное мечение каталога
// unsigned int mechenie_cataloga(unsigned int catalog) {
// 110100079 = это не иерархия
// 110100080 = нет данного каталога в каталоге иерархий
// 110100081 = это не иерархия
// 110100082 = нет данного каталога в каталоге ветвей
// 110100083 = это не каталог иерархий или ветвей
// 110100084 = пустой каталог
// 110100085 = некорректный каталог двоеточия в каталоге иерархий
// 110100086 = пустой каталог
// 110100087 = некорректный каталог двоеточия в каталоге ветвей
// 110100088 = пустой каталог
// 110100089 = нормальный выход
// поиск во вложенном каталоге ветвей заданного объекта в составе ветвей
// unsigned int poisk_v_cataloge_vetveq_object(unsigned int catalog,unsigned int object) {
// 110100090 = это не каталог ветвей
// 110100091 = это не иерархия
// 110100092 = это не ветвь
// 110100093 = нормальный выход
// поиск во вложенном каталоге ветвей заданной ветви
// unsigned int poisk_v_cataloge_vetveq_vetvi(unsigned int catalog,unsigned int vetv) {
// 110100094 = это не каталог ветвей
// 110100095 = это не ветвь
// 110100096 = это не иерархия
// 110100097 = это не ветвь
// 110100098 = нормальный выход
// поиск во вложенном каталоге ветвей заданного массива
// unsigned int poisk_v_cataloge_vetveq_massiva(unsigned int catalog,unsigned int *massiv) {
// 110100099 = это не каталог ветвей
// 110100100 = это не иерархия
// 110100101 = это не ветвь
// 110100102 = нормальный выход
// поиск в иерархии заданной ветви включенных в список вторичных объектов, возвращает либо адрес ячейки, либо 0
// unsigned int poisk_v_iera_vetvi(unsigned int iera,unsigned int vetv) {
// поиск в иерархии заданной ветви включенных в список вторичных объектов со стартового значения, возвращает либо адрес ячейки, либо 0
// unsigned int poisk_v_iera_vetvi_start(unsigned int iera,unsigned int vetv,
//  unsigned int start) {
// проверка включённости в иерархию заданной ветви
// unsigned int tochnoe_vkluchenie_vetvi_v_iera(unsigned int iera,unsigned int vetv) {
// 110100103 = это не иерархия
// 110100104 = это не ветвь
// 110100105 = некорректная длина иерархии
// 110100106 = некорректная количество записей иерархии
// 110100107 = некорректная длина ветви
// 110100108 = некорректная длина ветви
// 110100109 = ветвь есть;
// 110100110 = ветви нет
// 110100111 = ветви нет
// 110100112 = ветвь есть
// 110100113 = ветви нет
// поиск во вложенном каталоге иерархий заданной ветви
// unsigned int poisk_v_cataloge_iera_vetvi(unsigned int catalog,unsigned int vetv) {
// 110100114 = это не каталог иерархий
// 110100115 = это не иерархия
// 110100116 = это не ветвь
// 110100117 = это не иерархия
// 110100118 = нормальный выход
// поиск во вложенном каталоге иерархий иерархий с заданными творцовым объектом,иерархией источников и культурой объектов
// unsigned int poisk_v_cataloge_iera_harakteristik_iera(unsigned int catalog,
//  unsigned int poisk1,unsigned int poisk2,unsigned int poisk3,
//  unsigned int x1,unsigned int x2,unsigned int x3) {
// 110100119 = это не каталог иерархий
// 110100120 = некорректные параметры поиска
// 110100121 = это не иерархия
// 110100122 = это не иерархия
// 110100123 = нормальный выход
// поиск во вложенном каталоге ветвей ветвей с заданными творцовым объектом,иерархией источников и культурой объектов
// unsigned int poisk_v_cataloge_vetveq_harakteristik_vetveq(unsigned int catalog,
//  unsigned int poisk1,unsigned int poisk2,unsigned int poisk3,
//  unsigned int x1,unsigned int x2,unsigned int x3) {
// 110100124 = это не каталог ветвей
// 110100125 = некорректные параметры поиска
// 110100126 = это не иерархия
// 110100127 = это не ветвь
// 110100128 = нормальный выход
// поиск и мечение иерархии в каталоге по заданной категории
// unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_kategorii(unsigned int catalog,
//  unsigned int kategoriya) {
// 110100129 = это не иерархия
// 110100130 = это не иерархия
// 110100131 = нормальный выход
// поиск и мечение иерархии в каталоге по заданной категории и заданному первичному
// объекту
// unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu(
//  unsigned int catalog,unsigned int kategoriya,unsigned int object1) {
// 110100132 = это не иерархия
// 110100133 = это не иерархия
// 110100134 = нормальный выход
// поиск и мечение иерархии в каталоге по заданной категории и заданному первичному
// объекту и заданному вторичному объекту
// unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu(
//  unsigned int catalog,unsigned int kategoriya,unsigned int object1,
//  unsigned int object2) {
// 110100135 = это не иерархия
// 110100136 = это не иерархия
// 110100137 = нормальный выход
// поиск и мечение иерархии в каталоге по заданному вторичному объекту
// unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu(unsigned int catalog,
//  unsigned int object2) {
// 110100138 = это не каталог иерархий или ветвей
// 110100139 = это не иерархия
// 110100140 = это не иерархия
// 110100141 = нормальный выход
// поиск и мечение иерархии среди меченных иерархий по заданной категории
// unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii(unsigned int kategoriya) {
// 110100142 = это не иерархия
// 110100143 = это не иерархия
// 110100144 = нормальный выход
// поиск и мечение иерархии среди  меченных иерархий по заданной категории
// и заданному первичному объекту
// unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu(
//  unsigned int kategoriya,unsigned int object1) {
// 110100145 = это не иерархия
// 110100146 = это не иерархия
// 110100147 = нормальный выход
// поиск и мечение иерархии среди  меченных иерархий по заданной категории
// и заданному первичному объекту и заданному вторичному объекту
// unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu(
//  unsigned int kategoriya, unsigned int object1, unsigned int object2) {
// 110100148 = это не иерархия
// 110100149 = это не иерархия
// 110100150 = нормальный выход
// поиск и мечение иерархии среди  меченных иерархий по заданному вторичному объекту
// unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu(unsigned int object2) {
// 110100151 = это не иерархия
// 110100152 = это не иерархия
// 110100153 = нормальный выход
// снятие мечения каталога
// unsigned int snyatie_mecheniya_cataloga(unsigned int catalog) {
// 110100154 = это не иерархия
// 110100155 = нет данного каталога в каталоге иерархий
// 110100156 = это не иерархия
// 110100157 = нет данного каталога в каталоге ветвей
// 110100158 = это не каталог иерархий или ветвей
// 110100159 = пустой каталог
// 110100160 = некорректный каталог двоеточия в каталоге иерархий
// 110100161 = пустой каталог
// 110100162 = некорректный каталог двоеточия в каталоге ветвей
// 110100163 = пустой каталог
// 110100164 = нормальный выход
//сортировка ветвей в каталоге каталога ветвей по возрастанию цены
// unsigned int sortirovka_vetveq_v_cataloge(unsigned int catalog1, unsigned int oblast) {
// 110100165 = это не каталог ветвей
// 110100166 = это не иерархия
// 110100167 = нет свободной иерархии
// 110100168 = это не иерархия
// 110100169 = это не ветвь
// 110100170 = нет ветвей с нужной областью
// 110100171 = нормальный выход
// перенос меченого в каталог в каталоге иерархий или ветвей
// unsigned int perenos_mechenogo_v_catalog(unsigned int catalog2) {
// 110100172 = это не иерархия
// 110100173 = это не каталог иерархий или ветвей
// 110100174 = нормальный выход
// копирование меченого в каталог в каталоге иерархий или ветвей
// unsigned int kopirovanie_mechenogo_v_catalog(unsigned int catalog2) {
// 110100175 = это не иерархия
// 110100176 = это не каталог иерархий или ветвей
// 110100177 = нормальный выход
// перенос вложенного каталога в каталоге иерархий или ветвей
// unsigned int perenos_cataloga_v_catalog(unsigned int catalog2, unsigned int catalog1) {
// 110100178 = разные типы каталогов
// 110100179 = разные типы каталогов
// 110100180 = это не иерархия
// 110100181 = это не каталог иерархий или ветвей
// 110100182 = нормальный выход
// копирование вложенного каталога в каталоге иерархий или ветвей
// unsigned int kopirovanie_cataloga_v_catalog(unsigned int catalog2,
//  unsigned int catalog1) {
// 110100183 = разные типы каталогов
// 110100184 = разные типы каталогов
// 110100185 = это не иерархия
// 110100186 = это не каталог иерархий или ветвей
// 110100187 = нормальный выход
// извлечение иерархии каталога из каталога иерархий или ветвей в возвращаемом значении функции
// unsigned int izvlechenie_cataloga_v_iera(unsigned int catalog) {
// 0 = не тот номер иерархии
// 0 = это не иерархия
// 0 = это не иерархия
// 0 = это не каталог иерархий или ветвей
// подсчёт числа иерархий в каталоге в возвращаемом значении функции
// unsigned int chislo_iera_v_cataloge(unsigned int catalog) {
// подсчёт числа иерархий и каталогов в каталоге в возвращаемом значении функции
// unsigned int chislo_iera_i_catalogov_v_cataloge(unsigned int catalog) {
// удаление меченного
// unsigned int udalenie_mechenogo(unsigned int iera) {
// 110100188 = это не иерархия
// 110100189 = нормальный выход
// удаление каталога из каталога иерархий или каталога ветвей
// unsigned int udalenie_catalog_iz_cataloga(unsigned int catalog) {
// 110100190 = это не каталог иерархий или ветвей
// 110100191 = нет данного каталога в каталоге иерархий
// 110100192 = нет данного каталога в каталоге ветвей
// 110100193 = нормальный выход
// инициализация каталогов иерархий и ветвей, нужное чтобы установить массив адресов и массив имен
// unsigned int init_catalogi_iera(void) {
// 110100194 = нормальный выход
// инициализация иерархии massiv_iera[nomer_iera1]
// unsigned int init_iera(unsigned int nomer_iera1, 
//  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1) {
// 110100195 = это не иерархия
// 110100196 = нормальный выход
// инициализация иерархии massiv_iera[nomer_iera1] массивом x1
// unsigned int init_iera_massiv(unsigned int nomer_iera1, 
//  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1,
//  unsigned int *x1) {
// 110100197 = это не иерархия
// 110100198 = нормальный выход
// обнуление иерархии massiv_iera[nomer_iera1] до начального значения
// unsigned int ochistka_iera(unsigned int nomer_iera1) {
// 110100199 = это не иерархия
// 110100200 = нормальный выход
// обнуление сортировочного массива иерархии sortirovochnaya_iera[nomer_iera1]
// unsigned int ochistka_sortirovochnaya_iera(unsigned int nomer_iera1) {
// 110100201 = это не иерархия
// 110100202 = нормальный выход
// сортировка иерархии
// unsigned int sortirovka_iera(unsigned int nomer_iera1) {
// 110100203 = это не иерархия
// 110100204 = пустая иерархия
// 110100205 = есть первичный объект без связей
// 110100206 = всё впорядке,всё рассортированно
// 110100207 = невозможно сортировать по вторичным объектам
// иерархия 1 копируется в 2 и добавляется запись - опорный объект с массивом связей
// x1 - область объектов
// x2 - опорный объект
// x3 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// x4 - массив объектов связанных с опорным объектом
// unsigned int dozapis_end_iera_massiv(unsigned int nomer_iera2, unsigned int nomer_iera1,
//  unsigned int x1, unsigned int x2, unsigned int x3, unsigned int *x4) {
// 110100208 = это не иерархия
// 110100209 = некорректный массив
// 110100427 = выход за длину иерархии
// 110100210 = нормальный выход
// иерархия 1 копируется в 2 и добавляется запись - опорный объект с одной связью
// x1 - область объектов
// x2 - опорный объект
// x3 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// x4 - номер объекта связанного с опорным объектом
// unsigned int dozapis_end_iera_object(unsigned int nomer_iera2, unsigned int nomer_iera1,
//  unsigned int x1, unsigned int x2, unsigned int x3, unsigned int x4) {
// 110100211 = это не иерархия
// 110100212 = выход за длину иерархии
// 110100213 = нормальный выход
// иерархия 1 копируется в 2 и добавляется запись - опорный объект с нулем вторичных связей
// x1 - область объектов
// x2 - опорный объект
// x3 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// unsigned int dozapis_end_iera_object_zero(unsigned int nomer_iera2, unsigned int nomer_iera1,
//  unsigned int x1, unsigned int x2, unsigned int x3) {
// 110100214 = это не иерархия
// 110100215 = выход за длину иерархии
// 110100216 = нормальный выход
// добавление записи в каталог иерархий или ветвей
// unsigned int dozapis_v_catalog(unsigned int catalog, unsigned int object) {
// 110100219 = это не иерархия
// 110100220 = это не иерархия
// 110100221 = это не каталог каталога иерархий или ветвей
// 110100222 = нет данного каталога в каталоге иерархий
// 110100223 = нормальный выход
// 110100224 = нет данного каталога в каталоге ветвей
// 110100225 = нормальный выход
// 110100226 = нет данного каталога в каталоге иерархий
// 110100227 = нормальный выход
// 110100228 = нет данного каталога в каталоге ветвей
// 110100229 = нормальный выход
// определение точного включения 2й иерархии в 1ю без всяких хвостов, первая больше
// вывод кода если иерархии совпадают возвращает значение =1,  если несовпадают =0
// unsigned int tochnoe_vklychenie_iera(unsigned int nomer_iera1, unsigned int nomer_iera2) {
// 110100230 = это не иерархия
// 110100231 = это не иерархия
// 110100232 = иерархия 2 включает иерархию 1 в себе
// 110100233 = иерархия 2 не включает иерархию 1 в себе
// 110100234 = иерархия 2 включает иерархию 1 в себе
// удаление избыточности из иерархии 1, всё оставшееся переносится в иерархию 2
// unsigned int udalenie_lishnego_iera(unsigned int nomer_iera2, unsigned int nomer_iera1) {
// 110100235 = это не иерархия
// 110100236 = это не иерархия
// 110100237 = пустая иерархия
// 110100238 = иерархия очищена от избыточности
// 110100239 = иерархия очищена от избыточности
// удаление хоста из иерархии
// unsigned int udalenie_host_iz_iera(unsigned int iera2,unsigned int iera1,
//  unsigned int kategoriya,unsigned int host) {
// 110100240 = это не иерархия
// 110100241 = это не иерархия
// 110100242 = нет объекта
// 110100243 = нормальный выход
// уплотнение каталога иерархий
// unsigned int uplotnenie_cataloga_iera(void) {
// 110100244 = это не иерархия
// 110100245 = это пустая иерархия
// 110100246 = нет требуемой пустой иерархии
// 110100247 = ошибка уплотнения каталога
// 110100248 = нормальный выход
// уплотнение каталога ветвей
// unsigned int uplotnenie_cataloga_vetveq(void) {
// 110100249 = это не иерархия
// 110100250 = это пустая иерархия
// 110100251 = нет требуемой пустой иерархии
// 110100252 = ошибка уплотнения каталога
// 110100253 = нормальный выход
// склеивание двух иерархий, в результате третья иерархия
// unsigned int skleivanie_iera(unsigned int nomer_iera3
//  , unsigned int nomer_iera1, unsigned int nomer_iera2) {
// 110100254 = нет свободных иерархий
// 110100255 = это не иерархия
// 110100256 = это не иерархия
// 110100257 = нормальный выход
// склеивание двух иерархий, в результате третья иерархия, записываемая в первую
// unsigned int skleivanie_end_iera(unsigned int nomer_iera2, unsigned int nomer_iera1) {
// 110100258 = это не иерархия
// 110100259 = это не иерархия
// 110100260 = нормальный выход
// инициализация стартовой ветви дерева из иерархии по заданной области
// unsigned int init_vetvi_iera(unsigned int vetv1, unsigned int nomer_iera1,
//  unsigned int oblast_object) {
// 110100261 = это не иерархия
// 110100262 = это не ветвь
// 110100263 = нет записей в иерархии
// 110100264 = нормальный выход
// следующая ветвь иерархии
// unsigned int next_vetv_iera(unsigned int vetv2, unsigned int nomer_iera1,
// 110100265 = это не иерархия
// 110100266 = это не ветвь
// 110100267 = это не ветвь
// 110100268 = нет записей в иерархии
// 110100269 = пустая первичная ветвь
// 110100270 = нет полного соответствия ветви в иерархии
// 110100271 = дерево кончилось, все ветви перебраны

// на выходе функции длина строки в байтах
// unsigned int dlina_text_to_return(char *text) {
// конверсия текста в числовой массив
// unsigned int convert_text_to_massiv(char *text, int *massiv) {
// 110100301 = нормальный выход
// конверсия числового массива в текст
// unsigned int convert_massiv_to_text(int *massiv, char *text) {
// 110100302 = нормальный выход
// поиск символа в массиве, возвращает место символа
// 0 - некорректная длина массива
// 0 - отсутствие символа в массиве
// unsigned int find_simvola_v_massiv_to_return(unsigned int *massiv, unsigned char ch) {
// поиск int в массиве int, возвращает позицию символа
// 0 - некорректная длина массива int
// 0 - отсутствие int в массиве int
// unsigned int find_int_v_massiv_to_return(unsigned int *massiv, unsigned int ch) {
// поиск int в массиве int со стартовой позиции, возвращает позицию символа
// 0 - некорректная длина массива int
// 0 - некорректная стартовая позиция в массиве int
// 0 - отсутствие int в массиве int
// unsigned int find_int_v_massiv_next_to_return(unsigned int *massiv, unsigned int ch, unsigned int start) {
// поиск массивов2 в массиве1, результаты в массив3
// unsigned int find_massiv_v_massiv_to_massiv(unsigned int *massiv1, unsigned int *massiv2, unsigned int *massiv3) {
// 110100303 - некорректная длина массива1 int
// 110100304 - некорректная длина массива2 int
// 110100305 - некорректная длина массива2 int
// поиск массива2 в массиве1, результат выводится через return
// unsigned int find_massiv_v_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2) {
// 0 - некорректная длина массива1 int
// 0 - некорректная длина массива2 int
// 0 - некорректная длина массива2 int
// 0 - отсутствует массив в массиве
// поиск массива2 в массиве1 со стартовой позиции, результат выводится через return
// unsigned int find_massiv_v_massiv_next_to_return(unsigned int *massiv1, unsigned int *massiv2, unsigned int start) {
// 0 - некорректная длина массива1 int
// 0 - некорректная длина массива2 int
// 0 - некорректная длина массива2 int
// 0 - отсутствует массив в массиве
// определение кодировки массива, вовращает значение
// 27000051=koi8-r
// 27000052=cp1251
// 27000053=ibm866
// 27000054=utf8
// 27000055=utf16
// unsigned int opredelenie_kodirovki_to_return(unsigned int *massiv) {
// 110100306 - некорректная длина массива
// 110100307 - не определилась кодировка массива
// определение наличия набора символов в тексте, второй массив заполняется их порядковым номером в наборе
// unsigned int simvoly_v_text(unsigned int *massiv1, unsigned int *massiv_simvol,
//  unsigned int *massiv2) {
// 110100308 = нормальный выход
// конверсия текста text1 в text1 по таблицам символов из table1 в table2
// unsigned int convert_text1(unsigned int *text1, unsigned int *table1,
// 110100309 = нормальный выход
//  unsigned int *table2, unsigned int *text2) {
// добавление с правой части строки текста right_text_1
// unsigned int right_text(char *obschiq_text, char *right_text_1) {
// 110100310 = нормальный выход
// добавление с правой части массива right_massiv
// unsigned int right_massiv(unsigned int *obschiq_massiv, unsigned int *right_massiv) {
// 110100311 - некорректная длина массива int
// 110100312 - некорректная длина массива int
// 110100313 = нормальный выход
// добавление с левой части строки текста left_text_1
// unsigned int left_text(char *obschiq_text, char *left_text_1) {
// 110100314 = нормальный выход
// добавление с левой части массива left_massiv
// unsigned int left_massiv(unsigned int *obschiq_massiv, unsigned int *left_massiv) {
// 110100315 - некорректная длина массива int
// 110100316 - некорректная длина массива int
// 110100317 = нормальный выход
// склеивание строки из левой части, средней части и правой части текста
// unsigned int mid_text(char *left_text, char *mid_text, char *right_text, char *obschiq_text) {
// 110100318 = нормальный выход
// склеивание массива из левой части, средней части и правой части
// unsigned int mid_massiv(unsigned int *left_massiv, unsigned int *mid_massiv,
// 110100319 - некорректная длина массива int
// 110100320 - некорректная длина массива int
// 110100321 - некорректная длина массива int
// 110100322 = нормальный выход
//  unsigned int *right_massiv, unsigned int *obschiq_massiv) {
// вырезание справа n количества символов
// unsigned int right_rezalka_text(char *obschiq_text, unsigned int n, char *right_rezuemyq_text) {
// 110100323 - некорректная длина строки
// 110100324 = нормальный выход
// вырезание справа n количества элементов массива
// unsigned int right_rezalka_massiv(unsigned int *obschiq_massiv, unsigned int n,
// 110100325 - некорректная размер вырезания
// 110100326 = нормальный выход
//  unsigned int *right_rezuemyq_massiv) {
// вырезание слева n количества символов
// unsigned int left_rezalka_text(char *obschiq_text, unsigned int n, char *left_rezuemyq_text) {
// 110100327 - некорректная длина строки
// 110100328 = нормальный выход
// вырезание слева n количества элементов массива
// unsigned int left_rezalka_massiv(unsigned int *obschiq_massiv, unsigned int n,
//  unsigned int *left_rezuemyq_massiv) {
// 110100329 - некорректная размер вырезания
// 110100330 = нормальный выход
// сравнение двух строк, копия функции strcmp(x1,x2), но без необходимой для этого библиотеки string.h
// на выходе 1 - если строки совпадают 0 - если нет
// unsigned int sravnenie_text_to_return(char *text1, char *text2) {
// сравнение двух массивов
// на выходе 1 - если массивы совпадают 0 - если нет
// unsigned int sravnenie_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2) {
// нахождение включенности строки в строку
// на выходе 1 - если строка есть в строке 0 - если нет
// unsigned int vkluchennost_text_to_return(char *obschiq_text, char *vkluchennyq_text) {
// поиск включенности массива2 в массиве1, результат выводится через return
// 1 - присутствует массив в массиве
// 0 - отсутствует массив в массиве
// unsigned int vkluchennost_massiv_v_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2) {
// 110100424 - некорректная длина массива1 int
// 110100425 - некорректная длина массива2 int
// 110100426 - некорректная длина массива2 int
// нахождение включенности строки справа в строке
// на выходе 1 - если строка есть в строке 0 - если нет
// unsigned int vkluchennost_right_text_to_return(char *obschiq_text, char *vkluchennyq_right_text) {
// нахождение включенности массива2 справа в массиве1, результат выводится через return
// 1 - присутствует массив в массиве
// 0 - отсутствует массив в массиве
// unsigned int vkluchennost_right_massiv_to_return(unsigned int *obschiq_massiv, unsigned int 
// 110100332 - некорректная длина массива1 int
// 110100333 - некорректная длина массива2 int
// 110100334 - некорректная длина массива2 int
// нахождение включенности строки слева в строке
// на выходе 1 - если строка есть в строке 0 - если нет
// unsigned int vkluchennost_left_text_to_return(char *obschiq_text, char *vkluchennyq_left_text) {
// нахождение включенности массива2 слева в массиве1, результат выводится через return
// 1 - присутствует массив в массиве
// 0 - отсутствует массив в массиве
// unsigned int vkluchennost_left_massiv_to_return(unsigned int *obschiq_massiv, unsigned int 
// *vkluchennyq_left_massiv) {
// 110100335 - некорректная длина массива1 int
// 110100336 - некорректная длина массива2 int
// 110100337 - некорректная длина массива2 int
// разрезание строки на строки по разделительному символу '_'
// возвращает в return количество элементов
// 0 - некорректная длина строки
// unsigned int rezalka_text(char *text1, char *text2, char *text3, char *text4,
//  char *text5, char *text6, char *text7, char *text8, char *text9, char *text10,
//  char *obschiq_text) {
// разрезание массива на массивы по разделительному символу '_',
// возвращает в return количество элементов
// 0 - некорректная длина массива
// unsigned int rezalka_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2, 
//  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6, 
//  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10, 
//  unsigned int *obschiq_massiv) {
// подсчет количества непустых строк, вывод в return
// unsigned int kolichestvo_text_to_return(char *text1, char *text2, char *text3, char *text4,
//  char *text5, char *text6, char *text7, char *text8, char *text9, char *text10) {
// подсчет количества непустых массивов, вывод в return
// unsigned int kolichestvo_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
//  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
//  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10) {
// нахождения совпадения со строкой у группы строк
// на выходе 1 - если строка есть в хотя бы в одной из строк 0 - если нет
// unsigned int flag_sovpadeniya_text_to_return(char *text1, char *text2, char *text3, char *text4,
//  char *text5, char *text6, char *text7, char *text8, char *text9, char *text10, 
//  char *text_sovpadeniya) {
// нахождения совпадения с массивом у группы массивов
// на выходе 1 - если массив совпадает хотя бы с одним из массивов 0 - если нет
// unsigned int flag_sovpadeniya_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
//  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
//  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10,
//  unsigned int *massiv_sovpadeniya) {
// нахождения совпадения строки с правой строкой у группы строк
// на выходе 1 - если строка есть в правой строке 0 - если нет
// unsigned int flag_sovpadeniya_right_text_to_return(char *text1, char *text2, char *text3,
//  char *text4, char *text5, char *text6, char *text7, char *text8, char *text9,
//  char *text10, char *text_sovpadeniya) {
// нахождения совпадения массива с правым массивом у группы массивов
// на выходе 1 - если массив есть в правом массиве 0 - если нет
// unsigned int flag_sovpadeniya_right_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
//  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
//  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10,
//  unsigned int *massiv_sovpadeniya) {
// нахождения совпадения строки с левой строкой у группы строк
// на выходе 1 - если строка есть в левой строке 0 - если нет
// unsigned int flag_sovpadeniya_left_text_to_return(char *text1, char *text2, char *text3,
//  char *text4,char *text5, char *text6, char *text7, char *text8, char *text9,
//  char *text10, char *text_sovpadeniya) {
// нахождения совпадения массива с левым массивом у группы массивов
// на выходе 1 - если массив есть в левом массиве 0 - если нет
// unsigned int flag_sovpadeniya_left_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
//  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
//  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10,
//  unsigned int *massiv_sovpadeniya) {
// нахождения совпадения со строкой у группы строк
// на выходе 1 - если строка есть в строках 0 - если нет
// заполняется массив 1 - если строка есть в строке 0 - если нет
// unsigned int flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return( char *text1,
//  char *text2, char *text3,char *text4, char *text5, char *text6, char *text7,
//  char *text8, char *text9, char *text10,char *text_sovpadeniya, int *massiv) {
// нахождения совпадения массива с группой массивов
// на выходе 1 - если массив есть в каком либо из массивов 0 - если нет
// заполняется массив 1 - если массив совпадает с массивом 0 - если нет
// unsigned int flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return(
//  unsigned int *massiv1, unsigned int *massiv2, unsigned int *massiv3,
//  unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
//  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9,
//  unsigned int *massiv10, unsigned int *massiv_sovpadeniya, unsigned int *massiv) {
// копирование файла в тест, копирует весь файл включая нули
// unsigned int copy_file2text(char *massiv, char *file) {
// 110100338 - ошибка открытия файла
// 110100339 - текст нулевой длины
// 110100340 = нормальный выход
// копирование файла в массив int, копирует весь файл включая нули
// unsigned int copy_charfile2intmassiv(unsigned int *massiv, char *file) {
// 110100341 - ошибка открытия файла
// 110100342 - пустой массив
// 110100343 = нормальный выход
// копирование строки в файл, нули не копируются
// unsigned int copy_text2file(char *massiv, char *file) {
// 110100344 - ошибка открытия файла
// 110100345 - пустой текст
// 110100346 = нормальный выход
// копирование int массива в char файл
// unsigned int copy_intmassiv2charfile(int *massiv, char *file) {
// 110100347 - ошибка открытия файла
// 110100348 - пустой массив
// 110100349 = нормальный выход
// копирование int массива в int файл
// unsigned int copy_intmassiv2intfile(int *massiv, char *file) {
// 110100350 - ошибка открытия файла
// 110100351 - пустой массив
// 110100352 = нормальный выход
// копирование int файла в int массив
// unsigned int copy_file2intmassiv(int *massiv, char *file) {
// 110100353 - ошибка открытия файла
// 110100354 - пустой файл
// 110100355 = нормальный выход
// копирование текста в int массив
// unsigned int copy_text2intmassiv(int *massiv, char *text) {
// 110100356 - пустой текст
// 110100357 = нормальный выход
// копирование int массива в текст
// unsigned int copy_intmassiv2text(int *massiv, char *text) {
// 110100358 - некорректный массив
// 110100359 - пустой массив
// 110100360 = нормальный выход
// копирование двух int массивов в int массив
// unsigned int copy_intmassiv1_and_intmassiv2_2intmassiv3(int *massiv1, int *massiv2,
//  int *massiv3) {
// 110100361 - некоррекный первый массив
// 110100362 - некоррекный второй массив
// 110100363 - пустой массив
// 110100364 = нормальный выход
// копирование двух int массивов в первый int массив
// unsigned int copy_intmassiv1_and_intmassiv2_2intmassiv1(int *massiv1, int *massiv2) {
// 110100365 - некоррекный первый массив
// 110100366 - некоррекный второй массив
// 110100367 - пустой массив
// 110100368 = нормальный выход
// копирование текста1 и текста2 в текст3
// unsigned int copy_text1_and_text2_2text3(char *text1, char *text2, char *text3) {
// 110100369 - пустой текст
// 110100370 = нормальный выход
// копирование одного массива int в другой
// unsigned int copy_intmassiv1_to_intmassiv2(int *massiv1, int *massiv2) {
// 110100371 - некоррекный массив
// 110100372 - пустой массив
// 110100373 = нормальный выход
// копирование одной строки в другую
// unsigned int copy_text1_to_text2(char *text1, char *text2) {
// 110100374 - пустой текст
// 110100375 = нормальный выход
// выделение остатка cправа массива int и копирование его в другой массив int
// unsigned int ostatok_massiva_int_right(int *massiv1, int n, int *massiv2) {
// 110100376 - некорректный массив
// 110100377 - некорректный размер вырезания
// 110100378 - пустой второй массив
// 110100379 = нормальный выход
// выделение остатка слева массива int и копирование его в другой массив int
// unsigned int ostatok_massiva_int_left(int *massiv1, int n, int *massiv2) {
// 110100380 - некорректный массив
// 110100381 - некорректный размер вырезания
// 110100382 - пустой второй массив
// 110100383 = нормальный выход
// выделение остатка строки и копирование его в другую строку
// unsigned int ostatok_text_right(char *text1, int n, char *text2) {
// 110100384 - некорректный размер вырезания
// 110100385 - пустая вторая строка
// 110100386 = нормальный выход
// выделение остатка строки и копирование его в другую строку
// unsigned int ostatok_text_left(char *text1, int n, char *text2) {
// 110100387 - некорректный размер вырезания
// 110100388 - пустая вторая строка
// 110100389 = нормальный выход
// конверсия массива int по таблицам символов из table1 в table2
// unsigned int convert_int1(int *table1, int *table2, int *massiv1, int *massiv2) {
// 110100390 = нормальный выход
// конверсия из ibm866 в koi8r из файла в файл
// unsigned int convert_ibm866_to_koi8r_file1_to_file2(char *file1, char *file2) {
// 110100391 = нормальный выход
// конверсия из cp1251 в koi8r из файла в файл
// unsigned int convert_cp1251_to_koi8r_file1_to_file2(char *file1, char *file2) {
// 110100392 = нормальный выход
// конверсия из koi8r в cp1251 из файла в файл
// unsigned int convert_koi8r_to_cp1251_file1_to_file2(char *file1, char *file2) {
// 110100393 = нормальный выход
// конверсия массива int по таблицам символов из utf8 в koi8r
// unsigned int convert_int_utf8_to_koi8r(int *massiv1, int *massiv2) {
// 110100394 = нормальный выход
// конверсия из файла в файл по таблицам символов из utf8 в koi8r
// unsigned int convert_utf8_to_koi8r_file1_to_file2(char *file1, char *file2) {
// 110100395 = нормальный выход
// удаление из массива определенного символа
// unsigned int ubiranie_simvola(unsigned int *massiv, unsigned int simvol) {
// 110100396 - некорректная длина массива int
// 110100397 = нормальный выход
// удаление из массива символа 13
// unsigned int ubiranie_simvola_13(unsigned int *massiv) {
// 110100398 - некорректная длина массива int
// 110100399 = нормальный выход
// добавление в массиве к 10 символу символа 13
// unsigned int dobavlenie_simvola_13(unsigned int *massiv1, unsigned int *massiv2) {
// 110100399 - некорректная длина массива int
// 110100400 = нормальный выход
// заполнение упрощенного формата иерархии текстовым массивом
// unsigned int zapolnenie_prostoq_iera_text_massivom(unsigned int iera,
//  unsigned int *massiv, unsigned int pervichnyq_object) {
// 110100401 - некорректная длина массива int
// 110100402 = нет данной категории
// 110100403 = некорректная иерархия
// 110100404 = выход за длину иерархии
// 110100405 = нормальный выход
// заполнение массива int текстом с клавиатуры
// unsigned int zapolnenie_massiva_int_textom_keyboard(unsigned int *massiv) {
// 110100406 = некорректная строка
// 110100407 = нормальный выход
// установка флага системы информирующего о том что есть данные в массиве клавиатуры
// void system_flag1(int sig) {
// вывод на экран int массива как текста
// unsigned int vyvod_massiva_int(unsigned int *massiv) {
// 110100408 = нормальный выход
// вывод на экран char массива
// unsigned int vyvod_massiva_char(char *massiv) {
// 110100409 = нормальный выход
// вывод на экран символа перевода строки
// unsigned int vyvod_lf(void) {
// 110100410 = нормальный выход
// перевод текстового массива в массив int
// unsigned int preobrazovanie_massiva_char_v_massiv_int(char *massiv_char,
//  unsigned int *massiv_int) {
// 110100411 = нормальный выход
// перевод массива int в текстовый массив
// unsigned int preobrazovanie_massiva_int_v_massiv_char(unsigned int *massiv_int,
//  char *massiv_char) {
// 110100412 = ошибка массива
// 110100413 = нормальный выход
// помещение в текстовый массив справочной информации монитора
// unsigned int monitor_help_info(char *massiv_char2) {
// 110100414 = нормальный выход
// перевод с определенной позиции int массива в текстовую строку
// unsigned int perevod_int_massiva_pozition_v_text(unsigned int *massiv, unsigned int n,
//  char *text) {
// 110100415 = некорректный массив
// 110100416 = некорректная стартовая позиция
// 110100417 = нормальный выход
// перевод текстовой строки в строку с путем к файлу и в строку с именем файла
// unsigned int perevod_text_v_namepath_i_namefile(char *text, char *namepath,
//  char *namefile) {
// 110100418 = ошибочная строка
// 110100419 = ошибочная строка
// 110100420 = ошибочная строка
// 110100421 = ошибочная строка
// 110100422 = нормальный выход
// перевод десятичной текстовой цифры в массиве int в int цифру возращаемую в return
// unsigned int perevod_text_10cifry_v_int_to_return(unsigned int *massiv) {
// перевод шестнадцатиричной текстовой цифры в массиве int в int цифру возращаемую в return
// unsigned int perevod_text_16cifry_v_int_to_return(unsigned int *massiv) {
// перевод int цифры в int массив шестнадцатиричной текстовой цифры
// unsigned int perevod_int_cifry_v_int_16rich_massiv(unsigned int *massiv,
//   unsigned int cifra) {
// 110100423 = нормальный выход
// запись состояния УЭС
// unsigned int save_sostoyanie_UES(void) {
// 110100430 = ошибка открытия файла
// 110100431 = нормальный выход
// unsigned int load_sostoyanie_UES(void) {
// 110100432 = ошибка открытия файла
// 110100433 = ошибка чтения файла
// 110100434 = ошибка в файле
// 110100435 = ошибка в заголовке
// 110100436 = нормальный выход
// перенос в массив начала и длины текста ограниченного справа разделительными
// символами, перенос в отдельный массив причастных к этому разделительных символов
// в формате номер эквивалентной строки, далее тип разделительного символа
// unsigned int index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol(
//   unsigned int *massiv4, unsigned int *massiv3, unsigned int *massiv2,
//   unsigned int *massiv1) {
// 110100437 = нормальный выход
// вырезание в массив текста массива со стартовой позиции по определенную длину
// unsigned int vyrezalka_so_start_po_dlinu(unsigned int *massiv2, unsigned int *massiv1,
//   unsigned int start, unsigned int dlina) {
// 110100438 = некорректный старт
// 110100439 = некорректная длина
// 110100440 = нормальный выход
// инициализация и освобождение массивов BZ100000
// unsigned int init_i_clean_BZ100000(void) {
// 110100441 = нормальный выход
// выделение номера первого свободного массива BZ100000, если нет тогда возвращает 0
// unsigned int vydel_1_free_BZ100000(void) {
// 0 = нет свободного массива BZ100000
// печать int массива по символам
// unsigned int print_int_text_massiv(unsigned int *massiv) {
// 110100446 = нормальный выход;
// печать шестнадцатиричного значения int
// unsigned int print_16_int(unsigned int cifra) {
// 110100447 = нормальный выход;
// преобразование простого текста в форматированный в массиве
//  unsigned int copy_unformat_text_to_UES_text(unsigned int *massiv2,
//    unsigned int *massiv1, unsigned int text_codepage) {
// 110100448 = пустой массив
// 110100449 = некорректная кодовая страница    
// 110100450 = нормальный выход;
// преобразование простого текста в форматированный в массиве
//  unsigned int copy_UES_text_to_unformat_text(unsigned int *massiv2,
//    unsigned int *massiv1) {
// 110100451 = пустой массив
// 110100452 = нестандартный символ
// 110100453 = нормальный выход;
// копирование текстового массива в иерархию 
//  unsigned int copy_from_UES_text_to_iera(unsigned int iera,
//    unsigned int pervichnyq_object, unsigned int *massiv) {
// 110100454 = нет данной иерархии
// 110100455 = пустой массив
// 110100456 = нормальный выход
// копирование из иерархии в текстовый массив
//  unsigned int copy_from_iera_to_UES_text(unsigned int *massiv, unsigned int iera,
// 110100457 = нет данной иерархии
// 110100458 = нет записи
// 110100459 = нет вторичных объектов
// 110100460 = нормальный выход
// печать на экран записи иерархии по адресу записи
//  unsigned int print_zapis(unsigned int iera, unsigned int adres_zapis) {
// 110100461 = это не иерархия
// 110100462 = нормальный выход
// печать на экран иерархии
//  unsigned int print_iera(unsigned int iera) {
// 110100463 = это не иерархия
// 110100464 = записей больше чем длина массива
// 110100465 = нормальный выход
// загрузка записи текущей иерархии цифрами
//  unsigned int zagruzka_zapis_int(unsigned int iera) {
// 110100466 = это не иерархия
// 110100467 = 0 - exit
// 110100468 = нормальный выход
// загрузка записи текущей иерархии банальным текстом
//  unsigned int zagruzka_zapis_text(unsigned int iera) {
// 110100469 = это не иерархия
// 110100470 = 0 - exit
// 110100471 = выход за длину иерархии
// 110100472 = нормальный выход

//=====
// 110100475

//======
// копирование системного массива в массив BZ100000
unsigned int copy_system_massiv_to_BZ100000(unsigned int massiv_BZ) {
 int i1,i2;
 i1=massiv_system_int_buffer[0];
 for(i2=0;i2<i1;i2++)
   BZ100000[massiv_BZ][i2]=massiv_system_int_buffer[i2];
 return 110100473;
 }

// копирование массива BZ100000 в системный массив
unsigned int copy_BZ100000_to_system_massiv(unsigned int massiv_BZ) {
 int i1,i2;
 i1=BZ100000[massiv_BZ][0];
 for(i2=0;i2<i1;i2++)
   massiv_system_int_buffer[i2]=BZ100000[massiv_BZ][i2];
 return 110100474;
 }

// склеивание массивов имен в общее имя каталога obschee_name_file
unsigned int skleivanie_imen_v_catalog1(void) {
 unsigned int i1,i2,i3;
 i3=0;
 i1=path1[0];
 for(i2=1;i2<i1;i2++) {
   obschee_name_catalog[i3]=path1[i2];
   i3++;
   }
 i1=path2[0];
 for(i2=1;i2<i1;i2++) {
   obschee_name_catalog[i3]=path2[i2];
   i3++;
   }
 i1=path3[0];
 for(i2=1;i2<i1;i2++) {
   obschee_name_catalog[i3]=path3[i2];
   i3++;
   }
 return 110100001;// 110100001 = нормальный выход
 }

//другой вариант склеивания массивов имен в общее имя  каталога obschee_name_file
unsigned int skleivanie_imen_v_catalog2(void) {
 unsigned int i1,i2,i3,i4;
 i3=0;
 i1=path1[0];
 for(i2=1;i2<i1;i2++) {
   obschee_name_catalog[i3]=path1[i2];
   i3++;
   }
 obschee_name_catalog[i3]=simvol_cataloga;
 i3++;
 for(i2=0;i2<max_dlina_dlinnogo_text;i2++) {
   i4=text1[i2];
   obschee_name_catalog[i3]=i4;
   if(i4==0)
     goto skleivanie_imen_v_catalog2_1;
   i3++;
   }
skleivanie_imen_v_catalog2_1:
 return 110100002;// 110100002 = нормальный выход
 }

// склеивание массивов имен в общее имя файла obschee_name_file
unsigned int skleivanie_imen_v_file1(void) {
 unsigned int i1,i2,i3;
 for(i2=0;i2<max_dlina_dlinnogo_text;i2++) {
   i1=obschee_name_catalog[i2];
   obschee_name_file[i2]=i1;
   if(i1==0)
     goto skleivanie_imen_v_file1;
   }
skleivanie_imen_v_file1:
 i1=name1[0];
 for(i3=1;i3<i1;i3++) {
   obschee_name_file[i2]=name1[i3];
   i2++;
   }
 obschee_name_file[i2]=0;
 return 110100003;// 110100003 = нормальный выход
 }

// заполнение технологического массива text1 другим текстовым массивом
unsigned int zapolnenie_text1(char *x1) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=x1[i1];
   if(i2==0) {
     text1[i1]=0;
     return 110100004;// 110100004 = нормальный выход
     }
   text1[i1]=i2;
   }
 return 110100005;// 110100005 = выход за максимальную длину длинного текста
 }

//заполнение технологического массива text2 другим текстовым массивом
unsigned int zapolnenie_text2(char *x1) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=x1[i1];
   if(i2==0) {
     text2[i1]=0;
     return 110100006;// 110100006 = нормальный выход
     }
   text2[i1]=i2;
   }
 return 110100007;// 110100007 = выход за максимальную длину длинного текста
 }

// вывод технологического массива text1 в другой текстовый массив
unsigned int vyvod_text(char *x1) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     x1[i1]=0;
     return 110100008;// 110100008 = нормальный выход
     }
   x1[i1]=i2;
   }
 return 110100009;// 110100009 = выход за максимальную длину длинного текста
 }

// заполнение технологическим массивом text1 массива path1 указывающего на первую часть имени
unsigned int zapolnenie1(void) {
 unsigned int i1,i2,i3,i4;
 i1=0;
 i4=1;
 for(i2=0;i2<max_dlina_dlinnogo_text;i2++) {
   i3=text1[i2];
   if(i3==0) {
     path1[0]=i4;
     return 110100010;// 110100010 = нормальный выход
     }
   path1[i4]=i3;
   i4++;
   }
 return 110100011;// 110100011 = выход за максимальную длину длинного текста
 }

// заполнение технологическим массивом text1 массива path2 указывающего на вторую часть имени
unsigned int zapolnenie2(void) {
 unsigned int i1,i2,i3,i4;
 i1=0;
 i4=1;
 for(i2=0;i2<max_dlina_dlinnogo_text;i2++) {
   i3=text1[i2];
   if(i3==0) {
     path2[0]=i4;
     return 110100012;// 110100012 = нормальный выход
     }
   path2[i4]=i3;
   i4++;
   }
 return 110100013;// 110100013 = выход за максимальную длину длинного текста
 }

// заполнение технологическим массивом text1 массива path3 указывающего на третью часть имени
unsigned int zapolnenie3(void) {
 unsigned int i1,i2,i3,i4;
 i1=0;
 i4=1;
 for(i2=0;i2<max_dlina_dlinnogo_text;i2++) {
   i3=text1[i2];
   if(i3==0) {
     path3[0]=i4;
     return 110100014;// 110100014 = нормальный выход
     }
   path3[i4]=i3;
   i4++;
   }
 return 110100015;// 110100015 = выход за максимальную длину длинного текста
 }

// заполнение технологическим массивом text1 массива name1 указывающего на последнюю часть имени
unsigned int zapolnenie4(void) {
 unsigned int i1,i2,i3,i4;
 i1=0;
 i4=1;
 for(i2=0;i2<max_dlina_dlinnogo_text;i2++) {
   i3=text1[i2];
   if(i3==0) {
     name1[0]=i4;
     return 110100016;// 110100016 = нормальный выход
     }
   name1[i4]=i3;
   i4++;
   }
 return 110100017;// 110100017 = выход за максимальную длину длинного текста
 }

// склеивание технологических текстов text1 и text2 в text1
unsigned int skleivanie_text_text1_i_text2(void) {
 unsigned int i1,i2,i3,i4;
 i2=0;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   if(text1[i1]==0)
     goto skleivanie_text_text1_i_text2_1;
   }
 return 110100018;// 110100018 = выход за максимальную длину длинного текста
skleivanie_text_text1_i_text2_1:
 i3=i1;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i4=text2[i1];
   if(i4==0) {
     text1[i3]=0;
     return 110100019;// 110100019 = нормальный выход
     }
   text1[i3]=i4;
   i3++;
   }
 return 110100020;// 110100020 = выход за максимальную длину длинного текста
 }

// заполнение технологического массива text2 другим текстовым массивом text1
unsigned int zapolnenie_text2_from_text1(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     text2[i1]=0;
     return 110100021;// 110100021 = нормальный выход
     }
   text2[i1]=i2;
   }
 return 110100022;// 110100022 = выход за максимальную длину длинного текста
 }

// заполнение технологического массива text1 другим текстовым массивом text2
unsigned int zapolnenie_text1_from_text2(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text2[i1];
   if(i2==0) {
     text1[i1]=0;
     return 110100023;// 110100023 = нормальный выход
     }
   text1[i1]=i2;
   }
 return 110100024;// 110100024 = выход за максимальную длину длинного текста
 }

// преобразование цифры в символ
char preobrazovanie_cifry_v_simvol1(int cifra) {
 char simvol1;
 if(cifra<10)
   return cifra+48;
 return cifra+55;
 }

// дозапись символа к массиву text1
unsigned int dozapis_simvol_k_text1(char simvol) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     text1[i1]=simvol;
     i1++;
     text1[i1]=0;
     return 110100025;// 110100025 = нормальный выход
     }
   }
 return 110100026;// 110100026 = выход за максимальную длину длинного текста
 }

// преобразование цифры в псевдокаталог cifra1 & cifra2 & cifra3 & cifra4
unsigned int preobrazovanie_cifry_v_psevdocatalog(int cifry) {
 int i1;
 int c1;
 char s1;
 i1=cifry;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra4[1]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra4[0]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra3[1]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra3[0]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra2[1]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra2[0]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra1[1]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 cifra1[0]=s1;
 return 110100027;// 110100027 = нормальный выход
 }


// преобразование псевдокаталога cifra1 в массив-каталог text1 типа /ff
unsigned int preobrazovanie_cifry1_v_text1(void) {
 text1[0]=cifra1[0];
 text1[1]=cifra1[1];
 text1[2]=0;
 return 110100028;// 110100028 = нормальный выход
 }

// преобразование псевдокаталога cifra1 & cifra2 в массив-каталог text1 типа /ff/ff
unsigned int preobrazovanie_cifry12_v_text1(void) {
 text1[0]=cifra1[0];
 text1[1]=cifra1[1];
 text1[2]=simvol_cataloga;
 text1[3]=cifra2[0];
 text1[4]=cifra2[1];
 text1[5]=0;
 return 110100029;// 110100029 = нормальный выход
  }

// преобразование псевдокаталога cifra1 & cifra2 & cifra3 в массив-каталог text1 типа /ff/ff/ff
unsigned int preobrazovanie_cifry123_v_text1(void) {
 text1[0]=cifra1[0];
 text1[1]=cifra1[1];
 text1[2]=simvol_cataloga;
 text1[3]=cifra2[0];
 text1[4]=cifra2[1];
 text1[5]=simvol_cataloga;
 text1[6]=cifra3[0];
 text1[7]=cifra3[1];
 text1[8]=0;
 return 110100030;// 110100030 = нормальный выход
 }

// преобразование псевдокаталога cifra1 & cifra2 & cifra3 & cifra4 в массив-каталог text1 типа /ff/ff/ff/ff
unsigned int preobrazovanie_cifry1234_v_text1(void) {
 text1[0]=cifra1[0];
 text1[1]=cifra1[1];
 text1[2]=simvol_cataloga;
 text1[3]=cifra2[0];
 text1[4]=cifra2[1];
 text1[5]=simvol_cataloga;
 text1[6]=cifra3[0];
 text1[7]=cifra3[1];
 text1[8]=simvol_cataloga;
 text1[9]=cifra4[0];
 text1[10]=cifra4[1];
 text1[11]=0;
 return 110100031;// 110100031 = нормальный выход
 }

// преобразование цифры в каталог ff/ff/ff/ff в массиве text1
unsigned int preobrazovanie_cifry_v_catalog(int cifry) {
 int i1;
 int c1;
 char s1;
 text1[11]=0;
 i1=cifry;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[11]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[10]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[8]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[7]=s1;
 i1>>=4;
 text1[6]=simvol_cataloga;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[5]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[4]=s1;
 i1>>=4;
 text1[3]=simvol_cataloga;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[2]=s1;
 i1>>=4;
 c1=i1%16;
 s1=preobrazovanie_cifry_v_simvol1(c1);
 text1[1]=s1;
 text1[0]=simvol_cataloga;
 return 110100032;// 110100032 = нормальный выход
 }

// преобразование цифры cifra1 в каталог ff в массиве text2
unsigned int preobrazovanie_cifry1_v_text2(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     text2[i1]=cifra1[0];
     i1++;
     text2[i1]=cifra1[1];
     i1++;
     text2[i1]=0;
     return 0;
     }
   text2[i1]=i2;
   }
 return 110100033;// 110100033 = нормальный выход
 }

// преобразование цифры cifra2 в каталог ff/ff в массиве text2
unsigned int preobrazovanie_cifry2_v_text2(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     text2[i1]=simvol_cataloga;// "/"
     i1++;
     text2[i1]=cifra2[0];
     i1++;
     text2[i1]=cifra2[1];
     i1++;
     text2[i1]=0;
     return 0;
     }
   text2[i1]=i2;
   }
 return 110100034;// 110100034 = нормальный выход
 }

// преобразование цифры cifra3 в каталог ff/ff/ff в массиве text2
unsigned int preobrazovanie_cifry3_v_text2(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     text2[i1]=simvol_cataloga;// "/"
     i1++;
     text2[i1]=cifra3[0];
     i1++;
     text2[i1]=cifra3[1];
     i1++;
     text2[i1]=0;
     return 110100035;// 110100035 = нормальный выход
     }
   text2[i1]=i2;
   }
 return 110100036;// 110100036 = выход за максимальную длину длинного текста
 }

// преобразование цифры cifra4 в каталог ff/ff/ff/ff в массиве text2
unsigned int preobrazovanie_cifry4_v_text2(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text1[i1];
   if(i2==0) {
     text2[i1]=simvol_cataloga;// "/"
     i1++;
     text2[i1]=cifra4[0];
     i1++;
     text2[i1]=cifra4[1];
     i1++;
     text2[i1]=0;
     return 110100037;// 110100037 = нормальный выход
     }
   text2[i1]=i2;
   }
 return 110100038;// 110100038 = выход за максимальную длину длинного текста
 }

// склеивание технологических текстов text1 и text2 в text3
unsigned int skleivanie_text_text1_i_text2_v_text3(void) {
 unsigned int i1,i2,i3,i4;
 i2=0;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   if(text1[i1]==0)
     goto skleivanie_text_text1_i_text2_v_text3_1;
   text3[i1]=text1[i1];
   }
 return 110100039;// 110100039 = выход за максимальную длину длинного текста
skleivanie_text_text1_i_text2_v_text3_1:
 i3=i1;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i4=text2[i1];
   if(i4==0) goto skleivanie_text_text1_i_text2_v_text3_2;
   text3[i3]=i4;
   i3++;
   }
 return 110100040;// 110100040 = выход за максимальную длину длинного текста
skleivanie_text_text1_i_text2_v_text3_2:
 i3++;
 text3[i3]=0;
 return 110100041;// 110100041 = нормальный выход
 }

// заполнение технологического массива text3 другим текстовым массивом text2
unsigned int zapolnenie_text3_from_text2(void) {
 unsigned int i1,i2;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text2[i1];
   if(i2==0) {
     text3[i1]=0;
     return 110100042;// 110100042 = нормальный выход
     return 0;
     }
   text3[i1]=i2;
   }
 return 110100043;// 110100043 = выход за максимальную длину длинного текста
 }

// заполнение технологического массива path2 другим текстовым массивом text2
unsigned int zapolnenie_path2_from_text2(void) {
 unsigned int i1,i2,i3;
 i3=1;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text2[i1];
   if(i2==0) {
     path2[i3]=0;
     path2[0]=i3;
     return 110100044;// 110100044 = нормальный выход
     }
   path2[i3]=i2;
   i3++;
   }
 return 110100045;// 110100045 = выход за максимальную длину длинного текста
 }

// заполнение технологического массива path3 другим текстовым массивом text2
unsigned int zapolnenie_path3_from_text2(void) {
 unsigned int i1,i2,i3;
 i3=1;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text2[i1];
   if(i2==0) {
     path3[i3]=0;
     path3[0]=i3;
     return 110100046;// 110100046 = нормальный выход
     }
   path3[i3]=i2;
   i3++;
   }
 return 110100047;// 110100047 = выход за максимальную длину длинного текста
 }

// заполнение технологического массива path1 другим текстовым массивом text3, обнуление path2,path3 и name1
unsigned int zapolnenie_path1_from_text2(void) {
 unsigned int i1,i2,i3;
 i3=1;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i2=text3[i1];
   if(i2==0) {
     path1[i3]=0;
     path1[0]=i3;
     path2[0]=0;
     path3[0]=0;
     name1[0]=0;
     return 110100048;// 110100048 = нормальный выход
     }
   path1[i3]=i2;
   i3++;
   }
 return 110100049;// 110100049 = выход за максимальную длину длинного текста
 }

// создание директория
unsigned int create_catalog1(void) {
 preobrazovanie_cifry1_v_text1();
 skleivanie_imen_v_catalog2();
 mkdir(obschee_name_catalog,511);
 preobrazovanie_cifry12_v_text1();
 skleivanie_imen_v_catalog2();
 mkdir(obschee_name_catalog,511);
 preobrazovanie_cifry123_v_text1();
 skleivanie_imen_v_catalog2();
 mkdir(obschee_name_catalog,511);
 preobrazovanie_cifry1234_v_text1();
 skleivanie_imen_v_catalog2();
 mkdir(obschee_name_catalog,511);
 return 110100050;// 110100050 = нормальный выход
 }

// поиск следующей записи
// функция возвращает или адрес следующей записи иерархии или 0 если записи нет
unsigned int poisk_sleduyscheq_zapisi(unsigned int iera, unsigned int start) {
 unsigned int i1;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории
 iera=iera-0x1000000;
 i1=start;
 if(i1+4>massiv_iera[iera][0])
   return 0;
 return i1+4+massiv_iera[iera][i1+3];
 }

// поиск записи по заданной категории,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii(unsigned int iera, unsigned int kategoriya) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i3=6;
poisk_kategorii1:
 if(massiv_iera[iera][i3]==kategoriya)
   return i3;
 i4=massiv_iera[iera][i3+3];
 i3=i3+4+i4;
 if(i3<i1)
   goto poisk_kategorii1;
 return 0;//0 = нет данной категории
 }

// поиск записи по заданной категории со стартового значения,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_start(unsigned int iera, unsigned int kategoriya,
  unsigned int start) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i3=start;
 if(i3>i1-4)
   return 0;// 0 = некорректное стартовое значение
poisk_kategorii1:
 if(massiv_iera[iera][i3]==kategoriya)
   return i3;
 i4=massiv_iera[iera][i3+3];
 i3=i3+4+i4;
 if(i3<i1)
   goto poisk_kategorii1;
 return 0;// 0 = нет данной категории
 }

// поиск записи по заданной категории и опорному объекту,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_i_opornogo_object(unsigned int iera, 
  unsigned int kategoriya, unsigned int object) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории или опорного объекта
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i3=6;
poisk_kategorii_i_opornogo_object1:
 if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==object)
   return i3;
 i4=massiv_iera[iera][i3+3];
 i3=i3+4+i4;
 if(i3<i1)
   goto poisk_kategorii_i_opornogo_object1;
 return 0;// 0 = нет данной категории или опорного объекта
 }

// поиск записи по заданной категории и опорному объекту со стартового значения,
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_i_opornogo_object_start(unsigned int iera, 
  unsigned int kategoriya, unsigned int object, unsigned int start) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории или опорного объекта
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i3=start;
poisk_kategorii_i_opornogo_object_start1:
 if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==object)
   return i3;
 i4=massiv_iera[iera][i3+3];
 i3=i3+4+i4;
 if(i3<i1)
   goto poisk_kategorii_i_opornogo_object_start1;
 return 0;// 0 = нет данной категории или опорного объекта
 }

// поиск записи по заданной категории и опорному объекту и вторичному объекту
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_i_opornogo_object_i_vtorichnogo_object(unsigned int iera, 
  unsigned int kategoriya, unsigned int object1, unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории или опорного объекта или вторичного объекта
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i3=6;
poisk_kategorii_i_opornogo_object_i_vtorichnogo_object1:
 if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==object1) {
   i5=massiv_iera[iera][i3+3];
   if(i5==0)
     goto poisk_kategorii_i_opornogo_object_i_vtorichnogo_object2;
   i6=i3+4+i5;
   for(i7=i3+4;i7<i6;i7++) {
     if(massiv_iera[iera][i7]==object2)
       return i3;
     }
   }
poisk_kategorii_i_opornogo_object_i_vtorichnogo_object2:
 i4=massiv_iera[iera][i3+3];
 i3=i3+4+i4;
 if(i3<i1)
   goto poisk_kategorii_i_opornogo_object_i_vtorichnogo_object1;
 return 0;// 0 = нет данной категории или опорного объекта или вторичного объекта
 }

// поиск записи по заданной категории и опорному объекту и вторичному объекту со стартового значения
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_i_opornogo_object_i_vtorichnogo_object_start(unsigned int iera, 
  unsigned int kategoriya, unsigned int object1, unsigned int object2,
  unsigned int start) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной категории или опорного объекта или вторичного объекта
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i3=start;
poisk_kategorii_i_opornogo_object_i_vtorichnogo_object1:
 if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==object1) {
   i5=massiv_iera[iera][i3+3];
   if(i5==0)
     goto poisk_kategorii_i_opornogo_object_i_vtorichnogo_object2;
   i6=i3+4+i5;
   for(i7=i3+4;i7<i6;i7++) {
     if(massiv_iera[iera][i7]==object2)
       return i3;
     }
   }
poisk_kategorii_i_opornogo_object_i_vtorichnogo_object2:
 i4=massiv_iera[iera][i3+3];
 i3=i3+4+i4;
 if(i3<i1)
   goto poisk_kategorii_i_opornogo_object_i_vtorichnogo_object1;
 return 0;// 0 = нет данной категории или опорного объекта или вторичного объекта
 }

// поиск записи по вторичному объекту
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_vtorichnogo_object(unsigned int iera, unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данного объекта
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i2=6;
poisk_vtorichnogo_object1:
 i3=massiv_iera[iera][i2+3];
 i4=i2+4+i3;
 if(i3==0)
   goto poisk_vtorichnogo_object2;
 for(i5=i2+4;i5<i4;i5++) {
   if(massiv_iera[iera][i5]==object2)
     return i2;
   }
poisk_vtorichnogo_object2:
 if(i4<i1) {
   i2=i4;
   goto poisk_vtorichnogo_object1;
   }
 return 0;// 0 = нет данного объекта
 }

// поиск записи по вторичному объекту со стартового значения
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_vtorichnogo_object_start(unsigned int iera, unsigned int object2,
 unsigned int start) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данного объекта
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i2=start;
poisk_vtorichnogo_object_start1:
 i3=massiv_iera[iera][i2+3];
 i4=i2+4+i3;
 if(i3==0)
   goto poisk_vtorichnogo_object_start2;
 for(i5=i2+4;i5<i4;i5++) {
   if(massiv_iera[iera][i5]==object2)
     return i2;
   }
poisk_vtorichnogo_object_start2:
 if(i4<i1) {
   i2=i4;
   goto poisk_vtorichnogo_object_start1;
   }
 return 0;// 0 = нет данного объекта
 }

// поиск записи по категории и вторичному объекту
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_i_vtorichnogo_object(unsigned int iera,
  unsigned int kategoriya, unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной иерархии
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i2=6;
poisk_vtorichnogo_object1:
 i3=massiv_iera[iera][i2+3];
 i4=i2+4+i3;
 if(massiv_iera[iera][i2]!=kategoriya)
   goto poisk_vtorichnogo_object2;
 if(i3==0)
   goto poisk_vtorichnogo_object2;
 for(i5=i2+4;i5<i4;i5++) {
   if(massiv_iera[iera][i5]==object2)
     return i2;
   }
poisk_vtorichnogo_object2:
 if(i4<i1) {
   i2=i4;
   goto poisk_vtorichnogo_object1;
   }
 return 0;// 0 = нет данного объекта
 }

// поиск записи по категории и вторичному объекту со стартового значения
// функция возвращает или адрес совпадающй записи или 0 если записи нет
unsigned int poisk_kategorii_i_vtorichnogo_object_start(unsigned int iera,
  unsigned int kategoriya, unsigned int object2, unsigned int start) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = нет данной иерархии
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i2=start;
poisk_vtorichnogo_object_start1:
 i3=massiv_iera[iera][i2+3];
 i4=i2+4+i3;
 if(massiv_iera[iera][i2]!=kategoriya)
   goto poisk_vtorichnogo_object_start2;
 if(i3==0)
   goto poisk_vtorichnogo_object_start2;
 for(i5=i2+4;i5<i4;i5++) {
   if(massiv_iera[iera][i5]==object2)
     return i2;
   }
poisk_vtorichnogo_object_start2:
 if(i4<i1) {
   i2=i4;
   goto poisk_vtorichnogo_object_start1;
   }
 return 0;// 0 = нет данного объекта
 }

// мечение отдельных иерархий и отдельных ветвей
unsigned int mechenie(unsigned int zapis) {
 unsigned int nomer_iera;
 unsigned int n1=0;
 unsigned int i1=0;
 if(zapis>=0x1000001 && zapis<=0x2000000) { //иерархия
   nomer_iera=massiv_adress_iera[1];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 110100051;// 110100051 = это не иерархия
   i1=poisk_vtorichnogo_object(nomer_iera,zapis);
   nomer_iera=nomer_iera-0x1000000;
   if(massiv_iera[nomer_iera][i1]!=v_cataloge_iera_iera)
     return 110100052;// 110100052 = это не иерархия в каталоге иерархий
   massiv_iera[nomer_iera][i1]=v_cataloge_iera_mech_iera;
   return 110100053;// 110100053 = нормальный выход
   }
 if(zapis>=0x3000001 && zapis<=0x4000000) { //ветвь
   nomer_iera=massiv_adress_iera[2];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 110100054;// 110100054 = это не иерархия
   i1=poisk_vtorichnogo_object(nomer_iera,zapis);
   nomer_iera=nomer_iera-0x1000000;
   if(massiv_iera[nomer_iera][i1]!=v_cataloge_vetveq_vetv)
     return 110100055;// 110100055 = это не ветвь в каталоге ветвей
   massiv_iera[nomer_iera][i1]=v_cataloge_vetveq_mech_catalog;
   return 110100056;// 110100056 = нормальный выход
   }
 return 110100057;// 110100057 = это не иерархия или ветвь в каталоге иерархий или ветвей
 }

// нахождение адреса первой меченой отдельной иерархии или отдельной ветви
unsigned int nahojdenie_mechenogo_object(unsigned int zapis) {
 unsigned int nomer_iera;
 unsigned int n1=0;
 unsigned int i1=0;
 if(zapis>=0x1000001 && zapis<=0x2000000) { //иерархия
   nomer_iera=massiv_adress_iera[1];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 0;// 0 = это не иерархия
   i1=poisk_vtorichnogo_object(nomer_iera,zapis);
   nomer_iera=nomer_iera-0x1000000;
   if(massiv_iera[nomer_iera][i1]!=v_cataloge_iera_mech_iera)
     return 0;// 0 = это не иерархия в каталоге иерархий
   return i1;
   }
 if(zapis>=0x3000001 && zapis<=0x4000000) { //ветвь
   nomer_iera=massiv_adress_iera[2];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 0;// 0 = это не иерархия
   i1=poisk_vtorichnogo_object(nomer_iera,zapis);
   nomer_iera=nomer_iera-0x1000000;
   if(massiv_iera[nomer_iera][i1]!=v_cataloge_vetveq_mech_vetv)
     return 0;// = это не ветвь в каталоге ветвей
   return i1;
   }
 return 0;// 0 = это не иерархия или ветвь в каталоге иерархий или ветвей
 }

// снятие мечения с отдельных иерархий или отдельных ветвей
unsigned int snyatie_mecheniya_s_object(unsigned int zapis) {
 unsigned int nomer_iera;
 unsigned int n1=0;
 unsigned int i1=0;
 if(zapis>=0x1000001 && zapis<=0x2000000) { //иерархия
   nomer_iera=massiv_adress_iera[1];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 0;// 0 = это не иерархия
   i1=poisk_vtorichnogo_object(nomer_iera,zapis);
   nomer_iera=nomer_iera-0x1000000;
   if(massiv_iera[nomer_iera][i1]!=v_cataloge_iera_mech_iera)
     return 0;// 0 = это не иерархия в каталоге иерархий
   massiv_iera[nomer_iera][i1]=v_cataloge_iera_iera;
   return i1;
   }
 if(zapis>=0x3000001 && zapis<=0x4000000) { //ветвь
   nomer_iera=massiv_adress_iera[2];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 0;// 0 = это не иерархия
   i1=poisk_vtorichnogo_object(nomer_iera,zapis);
   nomer_iera=nomer_iera-0x1000000;
   if(massiv_iera[nomer_iera][i1]!=v_cataloge_vetveq_mech_vetv)
     return 0;// = это не ветвь в каталоге ветвей
   massiv_iera[nomer_iera][i1]=v_cataloge_vetveq_vetv;
   return i1;
   }
 return 0;// 0 = это не иерархия или ветвь в каталоге иерархий или ветвей
 }

// нахождение ближайшей пустой иерархии, если нет то 0
unsigned int blijaqshaya_pustaya_iera(void) {
 unsigned int i1;
 i1=1;
blijaqshaya_pustaya_iera1:
 if(massiv_iera[i1][1]==svobodnaya_iera) goto blijaqshaya_pustaya_iera2;
 i1++;
 if(i1>max_chislo_iera)
   return 0;// 0 = нет пустой иерархии
 goto blijaqshaya_pustaya_iera1;
blijaqshaya_pustaya_iera2:
 return i1+0x1000000;
 }

// нахождение ближайшей пустой ветви, если нет то 0
unsigned int blijaqshaya_pustaya_vetv(void) {
 unsigned int i1;
 i1=1;
blijaqshaya_pustaya_vetv1:
 if(massiv_vetveq[i1][1]==svobodnaya_vetv) goto blijaqshaya_pustaya_vetv2;
 i1++;
 if(i1>max_chislo_vetveq)
   return 0;// 0 = нет пустой иерархии
 goto blijaqshaya_pustaya_vetv1;
blijaqshaya_pustaya_vetv2:
 return i1+0x3000000;
 }

// инициализация ветви massiv_vetveq[nomer_vetvi1]
unsigned int init_vetvi(unsigned int nomer_vetvi1, 
  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1,
  unsigned int oblast1) {
 if(0x3000001<nomer_vetvi1||nomer_vetvi1>0x4000000)
   return 110100058;// 110100058 =  это не ветвь
 massiv_vetveq[nomer_vetvi1][0]=8;//общая длина ветви
 massiv_vetveq[nomer_vetvi1][1]=zanyataya_vetv;
 massiv_vetveq[nomer_vetvi1][2]=tvorec1;
 massiv_vetveq[nomer_vetvi1][3]=istochniki1;
 massiv_vetveq[nomer_vetvi1][4]=kultura1;
 massiv_vetveq[nomer_vetvi1][5]=oblast1;
 massiv_vetveq[nomer_vetvi1][6]=0;
 massiv_vetveq[nomer_vetvi1][7]=0;
 return 110100059;// 110100059 = нормальный выход
 }

// инициализация ветви massiv_vetveq[nomer_vetvi1] массивом x1
unsigned int init_vetv_massiv(unsigned int vetv,
  unsigned int tvorec, unsigned int istochniki, unsigned int kultura,
  unsigned int oblast, unsigned int *x) {
 unsigned int len;
 unsigned int gran;
 unsigned int chislo_zapiseq_iera;
 unsigned int message_exit;
 unsigned int nomer_elementa_vetvi;
 unsigned int i1;
 if(vetv<0x3000001||vetv>0x4000000)
   return 110100060;// 110100060 = это не ветвь
 vetv=vetv-0x3000000;
 len=x[0];
 if(len==0)
   return 110100428;//110100428 = некорректный массив
 gran=7-1+len;
 if(gran>max_dlina_vetveq)
   return 110100429;//110100429 = некорректный массив
 nomer_elementa_vetvi=1;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=1;
 nomer_elementa_vetvi++;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=tvorec;
 nomer_elementa_vetvi++;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=istochniki;
 nomer_elementa_vetvi++;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=kultura;
 nomer_elementa_vetvi++;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=oblast;
 nomer_elementa_vetvi++;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=1;
 nomer_elementa_vetvi++;
 massiv_vetveq[vetv][nomer_elementa_vetvi]=len-1;
 nomer_elementa_vetvi++;
 for(i1=1;i1<len;i1++) {
   massiv_vetveq[vetv][nomer_elementa_vetvi]=x[i1];
   nomer_elementa_vetvi++;
   }
 massiv_vetveq[vetv][0]=nomer_elementa_vetvi;
 return 110100061;// 110100061 = нормальный выход
 }

// обнуление ветви massiv_vetveq[nomer_vetvi1] до начального значения
unsigned int ochistka_vetvi(unsigned int nomer_vetvi1) {
 int i1;
 if(nomer_vetvi1<0x3000001||nomer_vetvi1>0x4000000)
   return 110100062;// 110100062 = это не ветвь
 massiv_vetveq[nomer_vetvi1][0]=8;//общая длина ветви
 massiv_vetveq[nomer_vetvi1][6]=0;
 massiv_vetveq[nomer_vetvi1][7]=0;
 for(i1=7;i1<=max_dlina_vetveq;i1++)
   massiv_vetveq[nomer_vetvi1][i1]=0;
 return 110100063;// 110100063 = нормальный выход
 }

// unsigned int sortirovochnaya_iera[1000][199998][7];
// перевод важнейших параметров иерархии в сортировочную таблицу
// стандарт формата сортировочной таблицы иерархий:
// x0 = метка 1-избранная запись
// x1 = существование
// x2 = область объектов
// x3 = опорный объект
// x4 = параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// x5 = количество связей
// x6 = адрес записи в иерархии
unsigned int perevod_iera(unsigned int nomer_iera1) {
unsigned int nomer_zapisi1;
unsigned int chislo_zapiseq1;
unsigned int i1,i2=0;
unsigned int len;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100064;// 110100064 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq1=massiv_iera[nomer_iera1][2];
 if(chislo_zapiseq1<1) return 110100065;// 110100065 = это пустая иерархия
 nomer_zapisi1=1;
 for(i1=6;i1<len;i1++) {
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6]=i1;//x6 - адрес записи в иерархии
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0]=0;// x0 - метка 1=избранная запись
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1]=zanyataya_iera; //x1 - существование записи
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2]=massiv_iera[nomer_iera1][i1++];// x2 - область объектов
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]=massiv_iera[nomer_iera1][i1++];// x3 - опорный объект
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4]=massiv_iera[nomer_iera1][i1++];// x4 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
   i2=massiv_iera[nomer_iera1][i1];//количество связей
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]=i2;// x5 - количество связей
perevod_iera1:
   if(i2>0) {
     i1++;
     i2--;
     goto perevod_iera1;
     }
   nomer_zapisi1++;
   }
 return 110100066;// 110100066 = нормальный выход
 }

// удаление одной записи иерархии
unsigned int udalenie_zapisi(unsigned int iera, unsigned int adres_zapisi) {
 unsigned int dlina;
 unsigned int i1;
 unsigned int i2;
 unsigned int nomer_elementa1;
 unsigned int nomer_elementa2;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100067;// 110100067 = это не иерархия
 iera=iera-0x1000000;
 dlina=massiv_iera[iera][0];
 if(adres_zapisi>dlina-4)
   return 110100068;// 110100068 = некорректное стартовое значение
 i1=massiv_iera[iera][adres_zapisi+3];
 i2=adres_zapisi+4+i1;
 if(i1>dlina)
   return 110100069;// 110100069 = некорректный адрес записи
 nomer_elementa2=adres_zapisi;
 nomer_elementa1=i2;
udalenie_zapisi1:
 if(nomer_elementa1>=dlina)
    goto udalenie_zapisi2;
 massiv_iera[iera][nomer_elementa2]=massiv_iera[iera][nomer_elementa1];
 nomer_elementa1++;
 nomer_elementa2++;
 massiv_iera[iera][nomer_elementa2]=massiv_iera[iera][nomer_elementa1];
 nomer_elementa1++;
 nomer_elementa2++;
 massiv_iera[iera][nomer_elementa2]=massiv_iera[iera][nomer_elementa1];
 nomer_elementa1++;
 nomer_elementa2++;
 i1=massiv_iera[iera][nomer_elementa1];
 massiv_iera[iera][nomer_elementa2]=i1;
 nomer_elementa1++;
 nomer_elementa2++;
 for(i2=1;i2<=i1;i2++) {
   massiv_iera[iera][nomer_elementa2]=massiv_iera[iera][nomer_elementa1];
   nomer_elementa1++;
   nomer_elementa2++;
   }
 goto udalenie_zapisi1;
udalenie_zapisi2:
 massiv_iera[iera][0]=nomer_elementa2;
 i1=massiv_iera[iera][2];
 i1--;
 massiv_iera[iera][2]=i1;
 return 110100070;// 110100070 = нормальный выход
 }

// удаление вторичного объекта из записи иерархии
unsigned int udalenie_vtorichnogo_object1(unsigned int iera, 
  unsigned int kategoriya, unsigned int object1, unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 i1=poisk_kategorii_i_opornogo_object_i_vtorichnogo_object(iera,kategoriya,object1,object2);
 if(i1==0)
   return 110100071;// 110100071 = нет данного вторичного объекта
 if(iera<0x1000000||iera>=0x2000000)
   return 110100072;// 110100072 = это не иерархия
 iera=iera-0x1000000;
 i2=massiv_iera[iera][i1+3];
 if(i2==1) {
   udalenie_zapisi(iera+0x1000000,i1);
   return 110100073;// 110100073 = нормальный выход
   }
 i3=i1+4+i2;
 for(i4=i1+4;i4<i3;i4++) {
   if(massiv_iera[iera][i4]==object2)
     goto udalenie_vtorichnogo_object1;
   }
udalenie_vtorichnogo_object1:
 i6=massiv_iera[iera][0];
 i6--;
 for(i7=i4;i7<i6;i7++)
   massiv_iera[iera][i7]=massiv_iera[iera][i7+1];
 massiv_iera[iera][0]=i6;
 return 110100074;// 110100074 = нормальный выход
 }

// удаление вторичного объекта из записи иерархии 
unsigned int udalenie_vtorichnogo_object2(unsigned int iera, unsigned int zapis,
  unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100075;// 110100075 = это не иерархия
 iera=iera-0x1000000;
 if(zapis>i1)
   return 110100076;// 110100076 = некорректный адрес записи
 i2=massiv_iera[iera][zapis+3];
 if(i2==1) {
   udalenie_zapisi(iera+0x1000000,zapis);
   return 110100077;// 110100077 = нормальный выход
   }
 i3=zapis+4+i2;
 for(i4=zapis+4;i4<i3;i4++) {
   if(massiv_iera[iera][i4]==object2)
     goto udalenie_vtorichnogo_object1;
   }
udalenie_vtorichnogo_object1:
 i6=massiv_iera[iera][0];
 i6--;
 for(i7=i4;i7<i6;i7++)
   massiv_iera[iera][i7]=massiv_iera[iera][i7+1];
 massiv_iera[iera][0]=i6;
 return 110100078;// 110100078 = нормальный выход
 }

// вложенное мечение каталога
unsigned int mechenie_cataloga(unsigned int catalog) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int f1=0;
 unsigned int f2=0;
 unsigned int nomer_elementa;
 unsigned int catalog2;
 unsigned int vlojenost[1000000];
 unsigned int velichina_vlojenosti;
 if(catalog>=0x2000001 && catalog<=0x3000000) {//каталог иерархий
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
     return 110100079;// 110100079 = это не иерархия
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_iera_dvoetochie,catalog);
   nomer_iera1=nomer_iera1-0x1000000;
   if(f1==0)
     return 110100080;// 110100080 = нет данного каталога в каталоге иерархий
   }
 if(catalog>=0x4000001 && catalog<=0x5000000) {//каталог ветвей
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
     return 110100081;// 110100081 = это не иерархия
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_vetveq_dvoetochie,catalog);
   nomer_iera1=nomer_iera1-0x1000000;
   if(f1==0)
     return 110100082;// 110100082 = нет данного каталога в каталоге ветвей
   }
 if(n1==0)
   return 110100083;// 110100083 = это не каталог иерархий или ветвей
 catalog2=catalog;
 vlojenost[1]=catalog2;
 velichina_vlojenosti=1;
 if(n1==2)
   goto mechenie_cataloga5;
mechenie_cataloga1:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_iera_catalog,catalog2);
 if(nomer_elementa!=0) {
   vlojenost[velichina_vlojenosti]=catalog2;
   velichina_vlojenosti++;
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_iera_mech_catalog;
   f2=1;
   if(massiv_iera[nomer_iera1][nomer_elementa+3]==0)
     return 110100084;// 110100084 = пустой каталог
   catalog2=massiv_iera[nomer_iera1][nomer_elementa+4];
   nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_iera_dvoetochie,catalog2);
   if(nomer_elementa==0)
     return 110100085;// 110100085 = некорректный каталог двоеточия в каталоге иерархий
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_iera_mech_dvoetochie;
   goto mechenie_cataloga1;
   }
mechenie_cataloga2:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_iera_iera,catalog2);
 if(nomer_elementa==0)
   goto mechenie_cataloga3;
 massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_iera_mech_iera;
 f2=1;
 goto mechenie_cataloga2;
mechenie_cataloga3:
 velichina_vlojenosti--;
 if(velichina_vlojenosti<1) {
   goto mechenie_cataloga4;
   }
 catalog2=vlojenost[velichina_vlojenosti];
 goto mechenie_cataloga1;
mechenie_cataloga5:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_vetveq_catalog,catalog2);
 if(nomer_elementa!=0) {
   velichina_vlojenosti++;
   vlojenost[velichina_vlojenosti]=catalog2;
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_vetveq_mech_catalog;
   f2=1;
   if(massiv_iera[nomer_iera1][nomer_elementa+3]==0)
     return 110100086;// 110100086 = пустой каталог
   catalog2=massiv_iera[nomer_iera1][nomer_elementa+4];
   nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_vetveq_dvoetochie,catalog2);
   if(nomer_elementa==0)
     return 110100087;// 110100087 = некорректный каталог двоеточия в каталоге ветвей
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_vetveq_mech_dvoetochie;
   goto mechenie_cataloga5;
   }
mechenie_cataloga6:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_vetveq_vetv,catalog2);
 if(nomer_elementa==0)
   goto mechenie_cataloga7;
 massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_vetveq_mech_vetv;
 f2=1;
 goto mechenie_cataloga6;
mechenie_cataloga7:
 velichina_vlojenosti--;
 if(velichina_vlojenosti<1) {
   goto mechenie_cataloga4;
   }
 catalog2=vlojenost[velichina_vlojenosti];
 goto mechenie_cataloga5;
mechenie_cataloga4:
 if(f2==0)
   return 110100088;// 110100088 = пустой каталог
 return 110100089;// 110100089 = нормальный выход
 }

// поиск во вложенном каталоге ветвей заданного объекта в составе ветвей
unsigned int poisk_v_cataloge_vetveq_object(unsigned int catalog,unsigned int object) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int nomer_iera1;
 unsigned int dlina;
 if(catalog<0x4000001||catalog>0x5000000)
   return 110100090;// 110100090 = это не каталог ветвей
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[2];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100091;// 110100091 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 dlina=massiv_iera[nomer_iera1][0];
 i1=6;
poisk_v_cataloge_vetveq_object1:
 i2=massiv_iera[nomer_iera1][i1+3];
 i3=i1+4+i2;
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_catalog) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_catalog;
   goto poisk_v_cataloge_vetveq_object2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_dvoetochie) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_dvoetochie;
   goto poisk_v_cataloge_vetveq_object2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_vetv) {
   for(i4=i1+4;i4<i3;i4++) {
     i5=massiv_iera[nomer_iera1][i4];
     if(i5<0x3000001||i5>0x4000000)
       return 110100092;// 110100092 = это не ветвь
     i6=i5-0x3000000;
     i7=8+massiv_vetveq[i6][7];
     for(i8=8;i8<i7;i8++) {
       if(massiv_vetveq[i6][i8]==object)
         goto poisk_v_cataloge_vetveq_object2;
       }
     }
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_vetv;
   }
poisk_v_cataloge_vetveq_object2:
 if(i3>=dlina)
   goto poisk_v_cataloge_vetveq_object3;
 i1=i3;
 goto poisk_v_cataloge_vetveq_object1;
poisk_v_cataloge_vetveq_object3:
 return 110100093;// 110100093 = нормальный выход
 }

// поиск во вложенном каталоге ветвей заданной ветви
unsigned int poisk_v_cataloge_vetveq_vetvi(unsigned int catalog,unsigned int vetv) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5; 
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int nomer_iera1;
 unsigned int dlina;
 unsigned int object;
 unsigned int dlina_vetvi;
 if(catalog<0x3000001||catalog>0x4000000)
   return 110100094;// 110100094 = это не каталог ветвей
 if(vetv<0x4000001||vetv>0x5000000)
   return 110100095;// 110100095 = это не ветвь
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[2];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100096;// 110100096 = это не иерархия
 vetv=vetv-0x3000000;
 dlina_vetvi=massiv_vetveq[vetv][7];
 nomer_iera1=nomer_iera1-0x1000000;
 dlina=massiv_iera[nomer_iera1][0];
 i1=6;
poisk_v_cataloge_vetveq_vetvi1:
 i2=massiv_iera[nomer_iera1][i1+3];
 i3=i1+4+i2;
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_catalog) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_catalog;
   goto poisk_v_cataloge_vetveq_vetvi2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_dvoetochie) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_dvoetochie;
   goto poisk_v_cataloge_vetveq_vetvi2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_vetv) {
   for(i4=i1+4;i4<i3;i4++) {
     i5=massiv_iera[nomer_iera1][i4];
     if(i5<0x3000001||i5>0x4000000)
       return 110100097;// 110100097 = это не ветвь
     i6=i5-0x3000000;
     i7=massiv_vetveq[i6][7];
     i8=8+i7;
     if(dlina_vetvi!=i7)
       goto poisk_v_cataloge_vetveq_vetvi4;
     if(dlina_vetvi==0&&i7==0)
       goto poisk_v_cataloge_vetveq_vetvi2;
     for(i9=8;i9<i8;i9++) {
       if(massiv_vetveq[vetv][i9]!=massiv_vetveq[i6][i9]) {
         goto poisk_v_cataloge_vetveq_vetvi4;
	 }
       }
     }
   goto poisk_v_cataloge_vetveq_vetvi5;
poisk_v_cataloge_vetveq_vetvi4:
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_vetv;
poisk_v_cataloge_vetveq_vetvi5:
   ;
   }
poisk_v_cataloge_vetveq_vetvi2:
 if(i3>=dlina)
   goto poisk_v_cataloge_vetveq_vetvi3;
 i1=i3;
 goto poisk_v_cataloge_vetveq_vetvi1;
poisk_v_cataloge_vetveq_vetvi3:
 return 110100098;// 110100098 = нормальный выход
 }

// поиск во вложенном каталоге ветвей заданного массива
unsigned int poisk_v_cataloge_vetveq_massiva(unsigned int catalog,unsigned int *massiv) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int nomer_iera1;
 unsigned int dlina;
 unsigned int object;
 unsigned int dlina_massiva;
 if(catalog<0x4000001||catalog>0x5000000)
   return 110100099;// 110100099 = это не каталог ветвей
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[2];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100100;// 110100100 = это не иерархия
 dlina_massiva=massiv[0]-1;
 nomer_iera1=nomer_iera1-0x1000000;
 dlina=massiv_iera[nomer_iera1][0];
 i1=6;
poisk_v_cataloge_vetveq_massiva1:
 i2=massiv_iera[nomer_iera1][i1+3];
 i3=i1+4+i2;
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_catalog) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_catalog;
   goto poisk_v_cataloge_vetveq_massiva2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_dvoetochie) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_dvoetochie;
   goto poisk_v_cataloge_vetveq_massiva2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_vetv) {
   for(i4=i1+4;i4<i3;i4++) {
     i5=massiv_iera[nomer_iera1][i4];
     if(i5<0x3000001||i5>0x4000000)
       return 110100101;// 110100101 = это не ветвь
     i6=i5-0x3000000;
     i7=massiv_vetveq[i6][7];
     i8=8+i7;
     if(dlina_massiva!=i7)
       goto poisk_v_cataloge_vetveq_massiva4;
     for(i9=8;i9<i8;i9++) {
       i10=i9-7;
       if(i10>dlina_massiva)
         goto poisk_v_cataloge_vetveq_massiva4;
       if(massiv[i10]!=massiv_vetveq[i6][i9])
         goto poisk_v_cataloge_vetveq_massiva4;
       }
     }
   goto poisk_v_cataloge_vetveq_massiva5;
poisk_v_cataloge_vetveq_massiva4:
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_vetv;
poisk_v_cataloge_vetveq_massiva5:
   ;
   }
poisk_v_cataloge_vetveq_massiva2:
 if(i3>=dlina)
   goto poisk_v_cataloge_vetveq_massiva3;
 i1=i3;
 goto poisk_v_cataloge_vetveq_massiva1;
poisk_v_cataloge_vetveq_massiva3:
 return 110100102;// 110100102 = нормальный выход
 }

// поиск в иерархии заданной ветви включенных в список вторичных объектов, возвращает либо адрес ячейки, либо 0
unsigned int poisk_v_iera_vetvi(unsigned int iera,unsigned int vetv) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int dlina_iera;
 unsigned int dlina_vetvi;
 unsigned int kategoriya;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = это не иерархия
 if(vetv<0x3000001||vetv>0x4000000)
   return 0;// 0 = это не ветвь
 iera=iera-0x1000000;
 dlina_iera=massiv_iera[iera][0];
 if(dlina_iera<=6)
   return 0;// 0 = некорректная длина иерархии
 vetv=vetv-0x3000000;
 if(massiv_vetveq[vetv][0]<8)
   return 0;// 0 = некорректная длина ветви
 dlina_vetvi=massiv_vetveq[vetv][7];
 kategoriya=massiv_vetveq[vetv][5];
 i1=6;
poisk_v_iera_vetvi1:
 i2=massiv_iera[iera][i1+3];
 i3=i1+4;
 if(i2!=dlina_vetvi)
   goto poisk_v_iera_vetvi2;
 if(i2!=dlina_vetvi)
   goto poisk_v_iera_vetvi2;
 if(kategoriya!=massiv_iera[iera][i1])
   goto poisk_v_iera_vetvi2;
 if(i2==0&&dlina_vetvi==0)
   goto poisk_v_iera_vetvi2;
 for(i4=0;i4<i2;i4++) {
   if(massiv_iera[iera][i3+i4]!=massiv_vetveq[vetv][8+i4])
     goto poisk_v_iera_vetvi2;
   }
 return i1;
poisk_v_iera_vetvi2:
 i5=i3+i2;
 if(i5>=dlina_iera)
   goto poisk_v_iera_vetvi3;
 i1=i5;
 goto poisk_v_iera_vetvi1;
poisk_v_iera_vetvi3:
 return 0;
 }

// поиск в иерархии заданной ветви включенных в список вторичных объектов со стартового значения
unsigned int poisk_v_iera_vetvi_start(unsigned int iera,unsigned int vetv,
  unsigned int start) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int dlina_iera;
 unsigned int dlina_vetvi;
 unsigned int kategoriya;
 if(iera<0x1000000||iera>=0x2000000)
   return 0;// 0 = это не иерархия
 if(vetv<0x3000001||vetv>0x4000000)
   return 0;// 0 = это не ветвь
 iera=iera-0x1000000;
 dlina_iera=massiv_iera[iera][0];
 if(dlina_iera<=6)
   return 0;// 0 = некорректная длина иерархии
 vetv=vetv-0x3000000;
 if(massiv_vetveq[vetv][0]<8)
   return 0;// 0 = некорректная длина ветви
 dlina_vetvi=massiv_vetveq[vetv][7];
 kategoriya=massiv_vetveq[vetv][5];
 i1=start;
poisk_v_iera_vetvi_start1:
 i2=massiv_iera[iera][i1+3];
 i3=i1+4;
 if(i2!=dlina_vetvi)
   goto poisk_v_iera_vetvi_start2;
 if(i2!=dlina_vetvi)
   goto poisk_v_iera_vetvi_start2;
 if(kategoriya!=massiv_iera[iera][i1]) {
   goto poisk_v_iera_vetvi_start2;
   }
 if(i2==0&&dlina_vetvi==0)
   goto poisk_v_iera_vetvi_start2;
 for(i4=0;i4<i2;i4++) {
   if(massiv_iera[iera][i3+i4]!=massiv_vetveq[vetv][8+i4])
     goto poisk_v_iera_vetvi_start2;
   }
 return i1;
poisk_v_iera_vetvi_start2:
 i5=i3+i2;
 if(i5>=dlina_iera)
   goto poisk_v_iera_vetvi_start3;
 i1=i5;
 goto poisk_v_iera_vetvi_start1;
poisk_v_iera_vetvi_start3:
 return 0;
 }

// проверка включённости в иерархию заданной ветви
unsigned int tochnoe_vkluchenie_vetvi_v_iera(unsigned int iera,unsigned int vetv) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int dlina_iera;
 unsigned int dlina_vetvi;
 unsigned int kategoriya;
 unsigned int pervishnyq_object;
 unsigned int vtorichnyq_object;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100103;// 110100103 = это не иерархия
 if(vetv<0x3000001||vetv>0x4000000)
   return 110100104;// 110100104 = это не ветвь
 iera=iera-0x1000000;
 dlina_iera=massiv_iera[iera][0];
 if(dlina_iera<=6)
   return 110100105;// 110100105 = некорректная длина иерархии
 if(massiv_iera[iera][2]<1)
   return 110100106;// 110100106 = некорректная количество записей иерархии
 vetv=vetv-0x3000000;
 if(massiv_vetveq[vetv][0]<8)
   return 110100107;// 110100107 = некорректная длина ветви
 dlina_vetvi=massiv_vetveq[vetv][7];
 if(dlina_vetvi==0)
   return 110100108;// 110100108 = некорректная длина ветви
 kategoriya=massiv_vetveq[vetv][5];
 if(dlina_vetvi==1) {
   pervishnyq_object=massiv_vetveq[vetv][8];
   i3=6;
tochnoe_vkluchenie_vetvi_v_iera5:
   if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==pervishnyq_object)
     return 110100109;// 110100109 = ветвь есть;
   i3=i3+4+massiv_iera[iera][i3+3];
   if(i3<dlina_iera)
     goto tochnoe_vkluchenie_vetvi_v_iera5;
   return 110100110;// 110100110 = ветви нет
   }
//вариант когда количество объектов в ветви больше одного
 i2=1;
 if(dlina_vetvi==2)
   goto tochnoe_vkluchenie_vetvi_v_iera6;
 i1=dlina_vetvi-1;
tochnoe_vkluchenie_vetvi_v_iera1:
 i3=6;
 pervishnyq_object=massiv_vetveq[vetv][7+i2];
 vtorichnyq_object=massiv_vetveq[vetv][8+i2];
tochnoe_vkluchenie_vetvi_v_iera2:
 i4=massiv_iera[iera][i3+3];
 i5=i3+4+i4;
 if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==pervishnyq_object) {
   for(i6=i3+4;i6<i5;i6++) {
     if(massiv_iera[iera][i6]==vtorichnyq_object)
       goto tochnoe_vkluchenie_vetvi_v_iera3;
     }
   }
 i3=i5;
 if(i3<dlina_iera)
   goto tochnoe_vkluchenie_vetvi_v_iera2;
 return 110100111;// 110100111 = ветви нет
tochnoe_vkluchenie_vetvi_v_iera3:
 i2++;
 if(i2<i1)
   goto tochnoe_vkluchenie_vetvi_v_iera1;
tochnoe_vkluchenie_vetvi_v_iera6:
 pervishnyq_object=massiv_vetveq[vetv][7+i2];
 vtorichnyq_object=massiv_vetveq[vetv][8+i2];
 i3=6;
tochnoe_vkluchenie_vetvi_v_iera4:
 i4=massiv_iera[iera][i3+3];
 i5=i3+4+i4;
 if(massiv_iera[iera][i3]==kategoriya && massiv_iera[iera][i3+1]==pervishnyq_object) {
   for(i6=i3+4;i6<i5;i6++) {
     if(massiv_iera[iera][i6]==vtorichnyq_object)
       return 110100112;// 110100112 = ветвь есть
     }
   }
 i3=i5;
 if(i3<dlina_iera)
   goto tochnoe_vkluchenie_vetvi_v_iera4;
 return 110100113;// 110100113 = ветви нет
 }

// поиск во вложенном каталоге иерархий заданной ветви
unsigned int poisk_v_cataloge_iera_vetvi(unsigned int catalog,unsigned int vetv) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int nomer_iera1;
 unsigned int nomer_iera2;
 unsigned int dlina;
 unsigned int object;
 unsigned int dlina_massiva;
 if(catalog<0x2000001||catalog>0x3000000)
   return 110100114;// 110100114 = это не каталог иерархий
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100115;// 110100115 = это не иерархия
 if(vetv<0x3000001||vetv>0x4000000)
   return 110100116;// 110100116 = это не ветвь
 nomer_iera1=nomer_iera1-0x1000000;
 dlina=massiv_iera[nomer_iera1][0];
 i1=6;
poisk_v_cataloge_iera_vetvi1:
 i2=massiv_iera[nomer_iera1][i1+3];
 i3=i1+4+i2;
 i4=0;
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_iera_mech_catalog) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_iera_catalog;
   goto poisk_v_cataloge_iera_vetvi2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_iera_mech_dvoetochie) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_iera_dvoetochie;
   goto poisk_v_cataloge_iera_vetvi2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_iera_mech_iera) {
     for(i5=i1+4;i5<i3;i5++) {
       nomer_iera2=massiv_iera[nomer_iera1][i5];
       if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
         return 110100117;// 110100117 = это не иерархия
       i4=tochnoe_vkluchenie_vetvi_v_iera(nomer_iera2,vetv);
       if(i4==110100109||i4==110100112) {
         goto poisk_v_cataloge_iera_vetvi5;
         }
       }
poisk_v_cataloge_iera_vetvi4:
   massiv_iera[nomer_iera1][i1]=v_cataloge_iera_iera;
poisk_v_cataloge_iera_vetvi5:
   ;
   }
poisk_v_cataloge_iera_vetvi2:
 if(i3>=dlina)
   goto poisk_v_cataloge_iera_vetvi3;
 i1=i3;
 goto poisk_v_cataloge_iera_vetvi1;
poisk_v_cataloge_iera_vetvi3:

 return 110100118; // 110100118 = нормальный выход
 }

// поиск во вложенном каталоге иерархий иерархий с заданными творцовым объектом,иерархией источников и культурой объектов
unsigned int poisk_v_cataloge_iera_harakteristik_iera(unsigned int catalog,
  unsigned int poisk1,unsigned int poisk2,unsigned int poisk3,
  unsigned int x1,unsigned int x2,unsigned int x3) {
 unsigned int z1=0;
 unsigned int z2=0;
 unsigned int z3=0;
 unsigned int t1=v_cataloge_vetveq_mech_vetv;
 unsigned int t2=280;
 unsigned int t3=281;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int nomer_iera1;
 unsigned int nomer_iera2;
 unsigned int dlina;
 unsigned int object;
 unsigned int dlina_massiva;
 if(catalog<0x2000001||catalog>0x3000000)
   return 110100119;// 110100119 = это не каталог иерархий
 mechenie_cataloga(catalog);
 if(poisk1==t1||poisk2==t1||poisk3==t1) {
   z1=1;
   }
 if(poisk1==t2||poisk2==t2||poisk3==t2) {
   z2=1;
   }
 if(poisk1==t3||poisk2==t3||poisk3==t3) {
   z3=1;
   }
 if(z1==0&&z2==0&&z3==0)
   return 110100120;// 110100120 = некорректные параметры поиска
poisk_v_cataloge_iera_harakteristik_iera6:
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100121;// 110100121 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 dlina=massiv_iera[nomer_iera1][0];
 i1=6;
poisk_v_cataloge_iera_harakteristik_iera1:
 i2=massiv_iera[nomer_iera1][i1+3];
 i3=i1+4+i2;
 i4=0;
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_iera_mech_catalog) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_iera_catalog;
   goto poisk_v_cataloge_iera_harakteristik_iera2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_iera_mech_dvoetochie) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_iera_dvoetochie;
   goto poisk_v_cataloge_iera_harakteristik_iera2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_iera_mech_iera) {
   if(i3!=0) {
     for(i5=i1+4;i5<i3;i5++) {
       nomer_iera2=massiv_iera[nomer_iera1][i5];
       if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
         return 110100122;// 110100122 = это не иерархия
       nomer_iera2=nomer_iera2-0x1000000;
       if(massiv_iera[nomer_iera2][1]==svobodnaya_iera)
         goto poisk_v_cataloge_iera_harakteristik_iera4;
       i4=0;
       if(z1==1) {
         if(massiv_iera[nomer_iera2][3]==x1)
	   i4=1;
	 }
       if(z2==1) {
         if(massiv_iera[nomer_iera2][4]==x2) {
	   i4=2;
	   }
           else {
	     i4=0;
	     }
	 }
       if(z3==1) {
         if(massiv_iera[nomer_iera2][5]==x3) {
	   i4=3;
	   }
           else {
	     i4=0;
	     }
	 }
       if(i4!=0) {
         goto poisk_v_cataloge_iera_harakteristik_iera5;
         }
       }
     }
poisk_v_cataloge_iera_harakteristik_iera4:
   massiv_iera[nomer_iera1][i1]=v_cataloge_iera_iera;
poisk_v_cataloge_iera_harakteristik_iera5:
   ;
   }
poisk_v_cataloge_iera_harakteristik_iera2:
 if(i3>=dlina)
   goto poisk_v_cataloge_iera_harakteristik_iera3;
 i1=i3;
 goto poisk_v_cataloge_iera_harakteristik_iera1;
poisk_v_cataloge_iera_harakteristik_iera3:
 return 110100123; // 110100123 = нормальный выход
 }

// поиск во вложенном каталоге ветвей ветвей с заданными творцовым объектом,иерархией источников и культурой объектов
unsigned int poisk_v_cataloge_vetveq_harakteristik_vetveq(unsigned int catalog,
  unsigned int poisk1,unsigned int poisk2,unsigned int poisk3,
  unsigned int x1,unsigned int x2,unsigned int x3) {
 unsigned int z1=0;
 unsigned int z2=0;
 unsigned int z3=0;
 unsigned int t1=v_cataloge_vetveq_mech_vetv;
 unsigned int t2=280;
 unsigned int t3=281;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int nomer_iera1;
 unsigned int vetv;
 unsigned int dlina;
 unsigned int object;
 unsigned int dlina_massiva;
 if(catalog<0x4000001||catalog>0x5000000)
   return 110100124;// 110100124 = это не каталог ветвей
 i1=mechenie_cataloga(catalog);
 if(poisk1==t1||poisk2==t1||poisk3==t1) {
   z1=1;
   }
 if(poisk1==t2||poisk2==t2||poisk3==t2) {
   z2=1;
   }
 if(poisk1==t3||poisk2==t3||poisk3==t3) {
   z3=1;
   }
 if(z1==0&&z2==0&&z3==0)
   return 110100125;// 110100125 = некорректные параметры поиска
poisk_v_cataloge_vetveq_harakteristik_vetveq6:
 nomer_iera1=massiv_adress_iera[2];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100126;// 110100126 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 dlina=massiv_iera[nomer_iera1][0];
 i1=6;
poisk_v_cataloge_vetveq_harakteristik_vetveq1:
 i2=massiv_iera[nomer_iera1][i1+3];
 i3=i1+4+i2;
 i4=0;
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_catalog) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_catalog;
   goto poisk_v_cataloge_vetveq_harakteristik_vetveq2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_dvoetochie) {
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_dvoetochie;
   goto poisk_v_cataloge_vetveq_harakteristik_vetveq2;
   }
 if(massiv_iera[nomer_iera1][i1]==v_cataloge_vetveq_mech_vetv) {
   if(i3!=0) {
     for(i5=i1+4;i5<i3;i5++) {
       vetv=massiv_iera[nomer_iera1][i5];
       if(vetv<0x3000001||vetv>0x4000000)
         return 110100127;// 110100127 = это не ветвь
       vetv=vetv-0x3000000;
       if(massiv_vetveq[vetv][0]<8)
         goto poisk_v_cataloge_vetveq_harakteristik_vetveq4;
       i4=0;
       if(z1==1) {
         if(massiv_vetveq[vetv][2]==x1)
	   i4=1;
	 }
       if(z2==1) {
         if(massiv_vetveq[vetv][3]==x2) {
	   i4=2;
	   }
           else {
	     i4=0;
	     }
	 }
       if(z3==1) {
         if(massiv_vetveq[vetv][4]==x3) {
	   i4=3;
	   }
           else {
	     i4=0;
	     }
	 }
       if(i4!=0) {
         goto poisk_v_cataloge_vetveq_harakteristik_vetveq5;
         }
       }
     }
poisk_v_cataloge_vetveq_harakteristik_vetveq4:
   massiv_iera[nomer_iera1][i1]=v_cataloge_vetveq_vetv;
poisk_v_cataloge_vetveq_harakteristik_vetveq5:
   ;
   }
poisk_v_cataloge_vetveq_harakteristik_vetveq2:
 if(i3>=dlina)
   goto poisk_v_cataloge_vetveq_harakteristik_vetveq3;
 i1=i3;
 goto poisk_v_cataloge_vetveq_harakteristik_vetveq1;
poisk_v_cataloge_vetveq_harakteristik_vetveq3:
 return 110100128; // 110100128 = нормальный выход
 }

// поиск и мечение иерархии в каталоге по заданной категории
unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_kategorii(unsigned int catalog,
  unsigned int kategoriya) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100129;// 110100129 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0)
   goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii3;
 i3=i2+4+massiv_iera[nomer_iera1][i2+3];
 for(i4=i2+4;i4<i3;i4++) {
   i5=massiv_iera[nomer_iera1][i4];
   if(i5<0x1000000||i5>=0x2000000)
     return 110100130;// 110100130 = это не иерархия
   i6=i5-0x1000000;
   i7=6;
   dlina2=massiv_iera[i6][0];
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii2:
   if(massiv_iera[i6][i7]==kategoriya) {
     break;
     }
   i7=i7+4+massiv_iera[i6][i7+3];
   if(i7<dlina2)
     goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii1;
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii3:
 return 110100131; // 110100131 = нормальный выход
 }

// поиск и мечение иерархии в каталоге по заданной категории и заданному первичному
// объекту
unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu(
  unsigned int catalog,unsigned int kategoriya,unsigned int object1) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100132;// 110100132 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0) 
   goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu3;
 i3=i2+4;
 i4=i3+massiv_iera[nomer_iera1][i2+3];
 for(i5=i3;i5<i4;i5++) {
   i6=massiv_iera[nomer_iera1][i5];
   if(i6<0x1000000||i5>=0x2000000)
     return 110100133;// 110100133 = это не иерархия
   i7=i6-0x1000000;
   i8=6;
   dlina2=massiv_iera[i7][0];
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu2:
   if(massiv_iera[i7][i8]==kategoriya&&massiv_iera[i7][i8+1]==object1) {
     break;
     }
   i8=i8+4+massiv_iera[i7][i8+3];
   if(i8<dlina2)
     goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu1;
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu3:
 return 110100134; // 110100134 = нормальный выход
 }

// поиск и мечение иерархии в каталоге по заданной категории и заданному первичному
// объекту и заданному вторичному объекту
unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu(
  unsigned int catalog,unsigned int kategoriya,unsigned int object1,
  unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100135;// 110100135 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0) 
   goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu3;
 i3=i2+4+massiv_iera[nomer_iera1][i2+3];
 for(i4=i2+4;i4<i3;i4++) {
   i5=massiv_iera[nomer_iera1][i4];
   if(i5<0x1000000||i5>=0x2000000)
     return 110100136;// 110100136 = это не иерархия
   i6=i5-0x1000000;
   i7=6;
   dlina2=massiv_iera[i6][0];
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu2:
   if(massiv_iera[i6][i7]==kategoriya&&massiv_iera[i6][i7+1]==object1) {
     i8=i7+4;
     i9=i8+massiv_iera[i6][i7+3];
     for(i10=i8;i10<i9;i10++) {
       if(massiv_iera[i6][i10]==object2)
         goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu4;
       }
     }
   i7=i7+4+massiv_iera[i6][i7+3];
   if(i7<dlina2)
     goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu4:
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu1;
poisk_i_mechenie_ierarhiq_cataloga_po_kategorii_i_pervichomu_i_vtorichnomu3:
 return 110100137; // 110100137 = нормальный выход
 }

// поиск и мечение иерархии в каталоге по заданному вторичному объекту
unsigned int poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu(unsigned int catalog,
  unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 if(0x2000001>=catalog&&catalog<=0x3000000)
   goto poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu5;
 if(0x4000001<catalog||catalog>0x5000000)
   return 110100138;// 110100138 = это не каталог иерархий или ветвей
poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu5:
 mechenie_cataloga(catalog);
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100139;// 110100139 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0) 
   goto poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu3;
 i3=i2+4+massiv_iera[nomer_iera1][i2+3];
 for(i4=i2+4;i4<i3;i4++) {
   i5=massiv_iera[nomer_iera1][i4];
   if(i5<0x1000000||i5>=0x2000000)
     return 110100140;// 110100140 = это не иерархия
   i6=i5-0x1000000;
   i7=6;
   dlina2=massiv_iera[i6][0];
poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu2:
   i8=i7+4;
   i9=i8+massiv_iera[i6][i7+3];
   for(i10=i8;i10<i9;i10++) {
     if(massiv_iera[i6][i10]==object2)
       goto poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu4;
     }
   i7=i7+4+massiv_iera[i6][i7+3];
   if(i7<dlina2)
     goto poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu4:
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu1;
poisk_i_mechenie_ierarhiq_cataloga_po_vtorichnomu3:
 return 110100141; // 110100141 = нормальный выход
 }

// поиск и мечение иерархии среди меченных иерархий по заданной категории
unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii(unsigned int kategoriya) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100142;// 110100142 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0) 
   goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii3;
 i3=i2+4+massiv_iera[nomer_iera1][i2+3];
 for(i4=i2+4;i4<i3;i4++) {
   i5=massiv_iera[nomer_iera1][i4];
   if(i5<0x1000000||i5>=0x2000000)
     return 110100143;// 110100143 = это не иерархия
   i6=i5-0x1000000;
   i7=6;
   dlina2=massiv_iera[i6][0];
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii2:
   if(massiv_iera[i6][i7]==kategoriya) {
     break;
     }
   i7=i7+4+massiv_iera[i6][i7+3];
   if(i7<dlina2)
     goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii1;
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii3:
 return 110100144; // 110100144 = нормальный выход
 }

// поиск и мечение иерархии среди  меченных иерархий по заданной категории
// и заданному первичному объекту
unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu(
  unsigned int kategoriya,unsigned int object1) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100145;// 110100145 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0) 
   goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu3;
 i3=i2+4;
 i4=i3+massiv_iera[nomer_iera1][i2+3];
 for(i5=i3;i5<i4;i5++) {
   i6=massiv_iera[nomer_iera1][i5];
   if(i6<0x1000000||i5>=0x2000000)
     return 110100146;// 110100146 = это не иерархия
   i7=i6-0x1000000;
   i8=6;
   dlina2=massiv_iera[i7][0];
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu2:
   if(massiv_iera[i7][i8]==kategoriya&&massiv_iera[i7][i8+1]==object1) {
     break;
     }
   i8=i8+4+massiv_iera[i7][i8+3];
   if(i8<dlina2)
     goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu1;
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu3:
 return 110100147; // 110100147 = нормальный выход
 }

// поиск и мечение иерархии среди  меченных иерархий по заданной категории
// и заданному первичному объекту и заданному вторичному объекту
unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu(
  unsigned int kategoriya, unsigned int object1, unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100148;// 110100148 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0)
   goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu3;
 i3=i2+4+massiv_iera[nomer_iera1][i2+3];
 for(i4=i2+4;i4<i3;i4++) {
   i5=massiv_iera[nomer_iera1][i4];
   if(i5<0x1000000||i5>=0x2000000)
     return 110100149;// 110100149 = это не иерархия
   i6=i5-0x1000000;
   i7=6;
   dlina2=massiv_iera[i6][0];
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu2:
   if(massiv_iera[i6][i7]==kategoriya&&massiv_iera[i6][i7+1]==object1) {
     i8=i7+4;
     i9=i8+massiv_iera[i6][i7+3];
     for(i10=i8;i10<i9;i10++) {
       if(massiv_iera[i6][i10]==object2)
         goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu4;
       }
     }
   i7=i7+4+massiv_iera[i6][i7+3];
   if(i7<dlina2)
     goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu4:
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu1;
poisk_i_mechenie_mechenyh_ierarhiq_po_kategorii_i_pervichomu_i_vtorichnomu3:
 return 110100150; // 110100150 = нормальный выход
 }

// поиск и мечение иерархии среди  меченных иерархий по заданному вторичному объекту
unsigned int poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu(unsigned int object2) {
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int nomer_iera1;
 unsigned int dlina2;
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100151;// 110100151 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 i2=6;
poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu1:
 i2=poisk_kategorii_start(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,i2);
 if(i2==0) 
   goto poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu3;
 i3=i2+4+massiv_iera[nomer_iera1][i2+3];
 for(i4=i2+4;i4<i3;i4++) {
   i5=massiv_iera[nomer_iera1][i4];
   if(i5<0x1000000||i5>=0x2000000)
     return 110100152;// 110100152 = это не иерархия
   i6=i5-0x1000000;
   i7=6;
   dlina2=massiv_iera[i6][0];
poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu2:
   i8=i7+4;
   i9=i8+massiv_iera[i6][i7+3];
   for(i10=i8;i10<i9;i10++) {
     if(massiv_iera[i6][i10]==object2)
       goto poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu4;
     }
   i7=i7+4+massiv_iera[i6][i7+3];
   if(i7<dlina2)
     goto poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu2;
   massiv_iera[nomer_iera1][i2]=v_cataloge_iera_iera;
   }
poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu4:
 i2=i2+4+massiv_iera[nomer_iera1][i2+3];
 goto poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu1;
poisk_i_mechenie_mechenyh_ierarhiq_po_vtorichnomu3:
 return 110100153; // 110100153 = нормальный выход
 }

// снятие мечения каталога
unsigned int snyatie_mecheniya_cataloga(unsigned int catalog) {
 unsigned int i1;
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int f1=0;
 unsigned int f2=0;
 unsigned int nomer_elementa;
 unsigned int catalog2;
 unsigned int vlojenost[1000000];
 unsigned int velichina_vlojenosti;
 if(catalog>=0x2000001 && catalog<=0x3000000) {//каталог иерархий
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
     return 110100154;// 110100154 = это не иерархия
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_iera_dvoetochie,catalog);
   nomer_iera1=nomer_iera1-0x1000000;
   if(f1==0)
     return 110100155;// 110100155 = нет данного каталога в каталоге иерархий
   }
 if(catalog>=0x4000001 && catalog<=0x5000000) {//каталог ветвей
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
     return 110100156;// 110100156 = это не иерархия
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_vetveq_dvoetochie,catalog);
   nomer_iera1=nomer_iera1-0x1000000;
   if(f1==0)
     return 110100157;// 110100157 = нет данного каталога в каталоге ветвей
   }
 if(n1==0)
   return 110100158;// 110100158 = это не каталог иерархий или ветвей
 catalog2=catalog;
 vlojenost[1]=catalog2;
 velichina_vlojenosti=1;
 if(n1==2)
   goto snyatie_mecheniya_cataloga5;
snyatie_mecheniya_cataloga1:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_iera_mech_catalog,catalog2);
 if(nomer_elementa!=0) {
   vlojenost[velichina_vlojenosti]=catalog2;
   velichina_vlojenosti++;
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_iera_catalog;
   f2=1;
   if(massiv_iera[nomer_iera1][nomer_elementa+3]==0)
     return 110100159;// 110100159 = пустой каталог
   catalog2=massiv_iera[nomer_iera1][nomer_elementa+4];
   nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_iera_mech_dvoetochie,catalog2);
   if(nomer_elementa==0)
     return 110100160;// 110100160 = некорректный каталог двоеточия в каталоге иерархий
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_iera_dvoetochie;
   goto snyatie_mecheniya_cataloga1;
   }
snyatie_mecheniya_cataloga2:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_iera_mech_iera,catalog2);
 if(nomer_elementa==0)
   goto snyatie_mecheniya_cataloga3;
 massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_iera_iera;
 f2=1;
 goto snyatie_mecheniya_cataloga2;
snyatie_mecheniya_cataloga3:
 velichina_vlojenosti--;
 if(velichina_vlojenosti<1) {
   goto snyatie_mecheniya_cataloga4;
   }
 catalog2=vlojenost[velichina_vlojenosti];
 goto snyatie_mecheniya_cataloga1;
snyatie_mecheniya_cataloga5:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_vetveq_mech_catalog,catalog2);
 if(nomer_elementa!=0) {
   velichina_vlojenosti++;
   vlojenost[velichina_vlojenosti]=catalog2;
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_vetveq_catalog;
   f2=1;
   if(massiv_iera[nomer_iera1][nomer_elementa+3]==0)
     return 110100161;// 110100161 = пустой каталог
   catalog2=massiv_iera[nomer_iera1][nomer_elementa+4];
   nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_vetveq_mech_dvoetochie,catalog2);
   if(nomer_elementa==0)
     return 110100162;// 110100162 = некорректный каталог двоеточия в каталоге ветвей
   massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_vetveq_dvoetochie;
   goto snyatie_mecheniya_cataloga5;
   }
snyatie_mecheniya_cataloga6:
 nomer_elementa=poisk_kategorii_i_opornogo_object(nomer_iera1+0x1000000,v_cataloge_vetveq_mech_vetv,catalog2);
 if(nomer_elementa==0)
   goto snyatie_mecheniya_cataloga7;
 massiv_iera[nomer_iera1][nomer_elementa]=v_cataloge_vetveq_vetv;
 f2=1;
 goto snyatie_mecheniya_cataloga6;
snyatie_mecheniya_cataloga7:
 velichina_vlojenosti--;
 if(velichina_vlojenosti<1) {
   goto snyatie_mecheniya_cataloga4;
   }
 catalog2=vlojenost[velichina_vlojenosti];
 goto snyatie_mecheniya_cataloga5;
snyatie_mecheniya_cataloga4:
 if(f2==0)
   return 110100163;// 110100163 = пустой каталог
 return 110100164; // 110100164 = нормальный выход
 }

//сортировка ветвей в каталоге каталога ветвей по возрастанию цены
unsigned int sortirovka_vetveq_v_cataloge(unsigned int catalog1, unsigned int oblast) {
 unsigned int nomer_iera1;
 unsigned int nomer_iera2;
 unsigned int zapis_dir1;
 unsigned int zapis_dvoetochie1;
 unsigned int zapis_element1;
 unsigned int zapis_dir2;
 unsigned int zapis_dvoetochie2;
 unsigned int zapis_element2;
 unsigned int chislo_zapiseq;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int i11;
 unsigned int i12;
 unsigned int i13;
 unsigned int i14;
 unsigned int i15;
 unsigned int i16;
 unsigned int i17;
 unsigned int i18;
 unsigned int i19;
 unsigned int f1;
 unsigned int len;
 if(0x4000001>catalog1&&catalog1>0x5000000)
   return 110100165;// 110100165 = это не каталог ветвей
 nomer_iera1=massiv_adress_iera[2];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100166;// 110100166 = это не иерархия
 nomer_iera2=blijaqshaya_pustaya_iera();
 if(nomer_iera2==0)
   return 110100167;// 110100167 = нет свободной иерархии
 if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
   return 110100168;// 110100168 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 nomer_iera2=nomer_iera2-0x1000000;
 zapis_dir1=v_cataloge_vetveq_mech_catalog;
 zapis_dvoetochie1=v_cataloge_vetveq_mech_dvoetochie;
 zapis_element1=v_cataloge_vetveq_mech_vetv;
 zapis_dir2=v_cataloge_vetveq_catalog;
 zapis_dvoetochie2=v_cataloge_vetveq_dvoetochie;
 zapis_element2=v_cataloge_vetveq_vetv;
 mechenie_cataloga(catalog1);
 i1=3;//творцовый объект
 massiv_iera[nomer_iera2][i1]=massiv_iera[nomer_iera1][i1];
 i1++;//иерархия источников
 massiv_iera[nomer_iera2][i1]=massiv_iera[nomer_iera1][i1];
 i1++;//культура объектов
 massiv_iera[nomer_iera2][i1]=massiv_iera[nomer_iera1][i1];
 i1++;//начало первой записи
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq=0;
 i2=i1;
 i14=0;
sortirovka_vetveq_v_cataloge1:
 i3=massiv_iera[nomer_iera1][i1];
 i4=massiv_iera[nomer_iera1][i1+3];
 i5=i1+4+i4;
 i6=i2+4+i4;
 i11=massiv_iera[nomer_iera1][i1+1];
 if(i3==zapis_element1&&i11==catalog1) {
   for(i8=i1+4;i8<i5;i8++) {
     i15=massiv_iera[nomer_iera1][i8];
     if(i15<0x3000001||i15>0x4000000)
       return 110100169;// 110100169 = это не ветвь
     i16=i15-0x3000000;
     i14++;
     sort_vetveq[i14][0]=massiv_vetveq[i16][5];//область ветви
     sort_vetveq[i14][1]=massiv_vetveq[i16][6];//оценка ветви
     sort_vetveq[i14][2]=massiv_iera[nomer_iera1][i1+1];//опорный объект
     sort_vetveq[i14][3]=i15;//вторичный объект
     }
   goto sortirovka_vetveq_v_cataloge2;
   }
 if(i3==zapis_dir1) {
   i3=zapis_dir2;
   massiv_iera[nomer_iera1][i1]=i3;//категория
   goto sortirovka_vetveq_v_cataloge3;
   }
 if(i3==zapis_dvoetochie1) {
   i3=zapis_dvoetochie2;
   massiv_iera[nomer_iera1][i1]=i3;//категория
   goto sortirovka_vetveq_v_cataloge3;
   }
 if(i3==zapis_element1) {
   i3=zapis_element2;
   massiv_iera[nomer_iera1][i1]=i3;//категория
   }
sortirovka_vetveq_v_cataloge3:
 massiv_iera[nomer_iera2][i2]=i3;//категория
 i2++;
 massiv_iera[nomer_iera2][i2]=i11;//первичный объект
 i2++;
 massiv_iera[nomer_iera2][i2]=massiv_iera[nomer_iera1][i1+2];//цена
 i2++;
 massiv_iera[nomer_iera2][i2]=i4;//количество вторичных объектов
 i2++;
 for(i8=i1+4;i8<i5;i8++) {
   massiv_iera[nomer_iera2][i2]=massiv_iera[nomer_iera1][i8];
   i2++;
   }
 chislo_zapiseq++;
 i2=i6;
sortirovka_vetveq_v_cataloge2:
 if(i5<len) {
   i1=i5;
   goto sortirovka_vetveq_v_cataloge1;
   }
 f1=0;
 i16=0;
sortirovka_vetveq_v_cataloge4:
 for(i15=1;i15<=i14;i15++) {
   if(sort_vetveq[i15][0]==oblast) {
     f1=1;
     i16++;
     sort_vetveq[i16][1]=sort_vetveq[i15][1];
     sort_vetveq[i16][2]=sort_vetveq[i15][2];
     sort_vetveq[i16][3]=sort_vetveq[i15][3];
     goto sortirovka_vetveq_v_cataloge5;
     }
   i17=sort_vetveq[i15][2];
   massiv_iera[nomer_iera1][i17]=zapis_element2;
sortirovka_vetveq_v_cataloge5:
   ;
   }
 if(f1==0)
   return 110100170;// 110100170 = нет ветвей с нужной областью
sortirovka_vetveq_v_cataloge6:
 i13=1;
 i15=i13+1;
 f1=0;
sortirovka_vetveq_v_cataloge7:
 if(i15>i16)
   goto sortirovka_vetveq_v_cataloge8;
 i17=sort_vetveq[i13][1];
 i18=sort_vetveq[i15][1];
 if(i17>i18) {
   i19=i17;
   sort_vetveq[i13][1]=i18;
   sort_vetveq[i15][1]=i19;
   i19=sort_vetveq[i13][2];
   sort_vetveq[i13][2]=sort_vetveq[i15][2];
   sort_vetveq[i15][2]=i19;
   i19=sort_vetveq[i13][3];
   sort_vetveq[i13][3]=sort_vetveq[i15][3];
   sort_vetveq[i15][3]=i19;
   f1=1;
   }
 i13++;
 i15++;
 goto sortirovka_vetveq_v_cataloge7;
sortirovka_vetveq_v_cataloge8:
 if(f1==1)
   goto sortirovka_vetveq_v_cataloge6;
 for(i15=1;i15<=i16;i15++) {
   massiv_iera[nomer_iera2][i2]=zapis_element2;
   i2++;
   massiv_iera[nomer_iera2][i2]=sort_vetveq[i15][2];
   i2++;
   massiv_iera[nomer_iera2][i2]=1;
   i2++;
   massiv_iera[nomer_iera2][i2]=1;
   i2++;
   massiv_iera[nomer_iera2][i2]=sort_vetveq[i15][3];
   i2++;
   chislo_zapiseq++;
   }
sortirovka_vetveq_v_cataloge9:
 massiv_iera[nomer_iera2][2]=chislo_zapiseq;
 massiv_iera[nomer_iera2][1]=zanyataya_iera;
 massiv_iera[nomer_iera2][0]=i2;
 massiv_iera[nomer_iera1][1]=svobodnaya_iera;
 massiv_adress_iera[2]=nomer_iera2+0x1000000;
 return 110100171; // 110100171 = нормальный выход
 }

// перенос меченого в каталог в каталоге иерархий или ветвей
unsigned int perenos_mechenogo_v_catalog(unsigned int catalog2) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int zapis_dir1;
 unsigned int zapis_dvoetochie1;
 unsigned int zapis_element1;
 unsigned int zapis_dir2;
 unsigned int zapis_dvoetochie2;
 unsigned int zapis_element2;
 unsigned int chislo_zapiseq;
 unsigned int i1;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int len;
 if(0x2000001<=catalog2&&catalog2<=0x3000000) {
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   zapis_dir1=v_cataloge_iera_mech_catalog;
   zapis_dvoetochie1=v_cataloge_iera_mech_dvoetochie;
   zapis_element1=v_cataloge_iera_mech_iera;
   zapis_dir2=v_cataloge_iera_catalog;
   zapis_dvoetochie2=v_cataloge_iera_dvoetochie;
   zapis_element2=v_cataloge_iera_iera;
   goto perenos_mechenogo_v_catalog4;
   }
 if(0x4000001<=catalog2&&catalog2<=0x5000000) {
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   zapis_dir1=v_cataloge_vetveq_mech_catalog;
   zapis_dvoetochie1=v_cataloge_vetveq_mech_dvoetochie;
   zapis_element1=v_cataloge_vetveq_mech_vetv;
   zapis_dir2=v_cataloge_vetveq_catalog;
   zapis_dvoetochie2=v_cataloge_vetveq_dvoetochie;
   zapis_element2=v_cataloge_vetveq_vetv;
   }
perenos_mechenogo_v_catalog4:
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100172;// 110100172 = это не иерархия
 if(n1==0)
   return 110100173;// 110100173 = это не каталог иерархий или ветвей
 nomer_iera1=nomer_iera1-0x1000000;
 i1=6;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq=massiv_iera[nomer_iera1][2];
perenos_mechenogo_v_catalog1:
 i3=massiv_iera[nomer_iera1][i1];
 i4=massiv_iera[nomer_iera1][i1+3];
 i5=i1+4+i4;
 if(i3==zapis_dir1) {
   massiv_iera[nomer_iera1][i1]=zapis_dir2;//культура объектов
   massiv_iera[nomer_iera1][i1+1]=catalog2;//первичный объект
   goto perenos_mechenogo_v_catalog3;
   }
 if(i3==zapis_dvoetochie1) {
   massiv_iera[nomer_iera1][i1]=zapis_dvoetochie2;//культура объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera1][i8]=catalog2;
     }
   goto perenos_mechenogo_v_catalog3;
   }
 if(i3==zapis_element1) {
   massiv_iera[nomer_iera1][i1]=zapis_element2;//культура объектов
   massiv_iera[nomer_iera1][i1+1]=catalog2;
   }
perenos_mechenogo_v_catalog3:
 if(i5>=len)
   goto perenos_mechenogo_v_catalog2;
 i1=i5;
 goto perenos_mechenogo_v_catalog1;
perenos_mechenogo_v_catalog2:
 return 110100174; // 110100174 = нормальный выход
 }

// копирование меченого в каталог в каталоге иерархий или ветвей
unsigned int kopirovanie_mechenogo_v_catalog(unsigned int catalog2) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int zapis_dir1;
 unsigned int zapis_dvoetochie1;
 unsigned int zapis_element1;
 unsigned int zapis_dir2;
 unsigned int zapis_dvoetochie2;
 unsigned int zapis_element2;
 unsigned int chislo_zapiseq;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int len;
 if(0x2000001<=catalog2&&catalog2<=0x3000000) {
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   zapis_dir1=v_cataloge_iera_mech_catalog;
   zapis_dvoetochie1=v_cataloge_iera_mech_dvoetochie;
   zapis_element1=v_cataloge_iera_mech_iera;
   zapis_dir2=v_cataloge_iera_catalog;
   zapis_dvoetochie2=v_cataloge_iera_dvoetochie;
   zapis_element2=v_cataloge_iera_iera;
   goto kopirovanie_mechenogo_v_catalog4;
   }
 if(0x4000001<=catalog2&&catalog2<=0x5000000) {
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   zapis_dir1=v_cataloge_vetveq_mech_catalog;
   zapis_dvoetochie1=v_cataloge_vetveq_mech_dvoetochie;
   zapis_element1=v_cataloge_vetveq_mech_vetv;
   zapis_dir2=v_cataloge_vetveq_catalog;
   zapis_dvoetochie2=v_cataloge_vetveq_dvoetochie;
   zapis_element2=v_cataloge_vetveq_vetv;
   }
kopirovanie_mechenogo_v_catalog4:
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100175;// 110100175 = это не иерархия
 if(n1==0)
   return 110100176;// 110100176 = это не каталог иерархий или ветвей
 nomer_iera1=nomer_iera1-0x1000000;
 i1=6;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq=massiv_iera[nomer_iera1][2];
 i2=len;
kopirovanie_mechenogo_v_catalog1:
 i3=massiv_iera[nomer_iera1][i1];
 i4=massiv_iera[nomer_iera1][i1+3];
 i5=i1+4+i4;
 i6=i2+4+i4;
 if(i3==zapis_dir1) {
   massiv_iera[nomer_iera1][i1]=zapis_dir2;
   massiv_iera[nomer_iera1][i2]=zapis_dir2;//культура объектов
   massiv_iera[nomer_iera1][i2+1]=catalog2;
   massiv_iera[nomer_iera1][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera1][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera1][i7]=massiv_iera[nomer_iera1][i8];
     i7++;
     }
   chislo_zapiseq++;
   i2=i6;
   goto kopirovanie_mechenogo_v_catalog3;
   }
 if(i3==zapis_dvoetochie1) {
   massiv_iera[nomer_iera1][i1]=zapis_dvoetochie2;
   massiv_iera[nomer_iera1][i2]=zapis_dvoetochie2;//культура объектов
   massiv_iera[nomer_iera1][i2+1]=massiv_iera[nomer_iera1][i1+1];//первичный объект
   massiv_iera[nomer_iera1][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera1][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera1][i7]=catalog2;
     i7++;
     }
   chislo_zapiseq++;
   i2=i6;
   goto kopirovanie_mechenogo_v_catalog3;
   }
 if(i3==zapis_element1) {
   massiv_iera[nomer_iera1][i1]=zapis_element2;
   massiv_iera[nomer_iera1][i2]=zapis_element2;//культура объектов
   massiv_iera[nomer_iera1][i2+1]=catalog2;//первичный объект
   massiv_iera[nomer_iera1][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera1][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera1][i7]=massiv_iera[nomer_iera1][i8];
     i7++;
     }
   chislo_zapiseq++;
   i2=i6;
   }
kopirovanie_mechenogo_v_catalog3:
 if(i5>=len)
   goto kopirovanie_mechenogo_v_catalog2;
 i1=i5;
 goto kopirovanie_mechenogo_v_catalog1;
kopirovanie_mechenogo_v_catalog2:
 massiv_iera[nomer_iera1][2]=chislo_zapiseq;
 massiv_iera[nomer_iera1][0]=i2;
 return 110100177; // 110100177 = нормальный выход
 }

// перенос вложенного каталога в каталоге иерархий или ветвей
unsigned int perenos_cataloga_v_catalog(unsigned int catalog2, unsigned int catalog1) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int zapis_dir1;
 unsigned int zapis_dvoetochie1;
 unsigned int zapis_element1;
 unsigned int zapis_dir2;
 unsigned int zapis_dvoetochie2;
 unsigned int zapis_element2;
 unsigned int chislo_zapiseq;
 unsigned int i1;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int len;
 if(0x2000001<=catalog1&&catalog1<=0x3000000) {
   if(0x2000001>catalog2|catalog2>0x3000000)
     return 110100178;// 110100178 = разные типы каталогов
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   zapis_dir1=v_cataloge_iera_mech_catalog;
   zapis_dvoetochie1=v_cataloge_iera_mech_dvoetochie;
   zapis_element1=v_cataloge_iera_mech_iera;
   zapis_dir2=v_cataloge_iera_catalog;
   zapis_dvoetochie2=v_cataloge_iera_dvoetochie;
   zapis_element2=v_cataloge_iera_iera;
   goto perenos_cataloga_v_catalog4;
   }
 if(0x4000001<=catalog1&&catalog1<=0x5000000) {
   if(0x4000001>catalog2|catalog2>0x5000000)
     return 110100179;// 110100179 = разные типы каталогов
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   zapis_dir1=v_cataloge_vetveq_mech_catalog;
   zapis_dvoetochie1=v_cataloge_vetveq_mech_dvoetochie;
   zapis_element1=v_cataloge_vetveq_mech_vetv;
   zapis_dir2=v_cataloge_vetveq_catalog;
   zapis_dvoetochie2=v_cataloge_vetveq_dvoetochie;
   zapis_element2=v_cataloge_vetveq_vetv;
   }
perenos_cataloga_v_catalog4:
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100180;// 110100180 = это не иерархия
 if(n1==0)
   return 110100181;// 110100181 = это не каталог иерархий или ветвей
 nomer_iera1=nomer_iera1-0x1000000;
 mechenie_cataloga(catalog1);
 i1=6;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq=massiv_iera[nomer_iera1][2];
perenos_cataloga_v_catalog1:
 i3=massiv_iera[nomer_iera1][i1];
 i4=massiv_iera[nomer_iera1][i1+3];
 i5=i1+4+i4;
 if(i3==zapis_dir1) {
   massiv_iera[nomer_iera1][i1]=zapis_dir2;//культура объектов
   i9=massiv_iera[nomer_iera1][i1+1];//первичный объект
   i10=i9;
   if(i9==catalog1)
     i10=catalog2;
   massiv_iera[nomer_iera1][i1+1]=i10;
   goto perenos_cataloga_v_catalog3;
   }
 if(i3==zapis_dvoetochie1) {
   massiv_iera[nomer_iera1][i1]=zapis_dvoetochie2;//культура объектов
   for(i8=i1+4;i8<i5;i8++) {
     i9=massiv_iera[nomer_iera1][i8];
     if(i9==catalog1)
       i9=catalog2;
     massiv_iera[nomer_iera1][i8]=i9;
     }
   goto perenos_cataloga_v_catalog3;
   }
 if(i3==zapis_element1) {
   massiv_iera[nomer_iera1][i1]=zapis_element2;//культура объектов
   i9=massiv_iera[nomer_iera1][i1+1];//первичный объект
   i10=i9;
   if(i9==catalog1)
     i10=catalog2;
   massiv_iera[nomer_iera1][i1+1]=i10;
   }
perenos_cataloga_v_catalog3:
 if(i5>=len)
   goto perenos_cataloga_v_catalog2;
 i1=i5;
 goto perenos_cataloga_v_catalog1;
perenos_cataloga_v_catalog2:
 return 110100182; // 110100182 = нормальный выход
 }

// копирование вложенного каталога в каталоге иерархий или ветвей
unsigned int kopirovanie_cataloga_v_catalog(unsigned int catalog2,
  unsigned int catalog1) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int zapis_dir1;
 unsigned int zapis_dvoetochie1;
 unsigned int zapis_element1;
 unsigned int zapis_dir2;
 unsigned int zapis_dvoetochie2;
 unsigned int zapis_element2;
 unsigned int chislo_zapiseq;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int i9;
 unsigned int i10;
 unsigned int len;
 if(0x2000001<=catalog1&&catalog1<=0x3000000) {
   if(0x2000001>catalog2|catalog2>0x3000000)
     return 110100183;// 110100183 = разные типы каталогов
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   zapis_dir1=v_cataloge_iera_mech_catalog;
   zapis_dvoetochie1=v_cataloge_iera_mech_dvoetochie;
   zapis_element1=v_cataloge_iera_mech_iera;
   zapis_dir2=v_cataloge_iera_catalog;
   zapis_dvoetochie2=v_cataloge_iera_dvoetochie;
   zapis_element2=v_cataloge_iera_iera;
   goto kopirovanie_cataloga_v_catalog4;
   }
 if(0x4000001<=catalog1&&catalog1<=0x5000000) {
   if(0x4000001>catalog2|catalog2>0x5000000)
     return 110100184;// 110100184 = разные типы каталогов
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   zapis_dir1=v_cataloge_vetveq_mech_catalog;
   zapis_dvoetochie1=v_cataloge_vetveq_mech_dvoetochie;
   zapis_element1=v_cataloge_vetveq_mech_vetv;
   zapis_dir2=v_cataloge_vetveq_catalog;
   zapis_dvoetochie2=v_cataloge_vetveq_dvoetochie;
   zapis_element2=v_cataloge_vetveq_vetv;
   }
kopirovanie_cataloga_v_catalog4:
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100185;// 110100185 = это не иерархия
 if(n1==0)
   return 110100186;// 110100186 = это не каталог иерархий или ветвей
 nomer_iera1=nomer_iera1-0x1000000;
 mechenie_cataloga(catalog1);
 i1=6;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq=massiv_iera[nomer_iera1][2];
 i2=len;
kopirovanie_cataloga_v_catalog1:
 i3=massiv_iera[nomer_iera1][i1];
 i4=massiv_iera[nomer_iera1][i1+3];
 i5=i1+4+i4;
 i6=i2+4+i4;
 if(i3==zapis_dir1) {
   massiv_iera[nomer_iera1][i2]=zapis_dir2;//культура объектов
   i9=massiv_iera[nomer_iera1][i1+1];//первичный объект
   i10=i9;
   if(i9==catalog1)
     i10=catalog2;
   massiv_iera[nomer_iera1][i2+1]=i10;
   massiv_iera[nomer_iera1][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera1][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera1][i7]=massiv_iera[nomer_iera1][i8];
     i7++;
     }
   chislo_zapiseq++;
   i2=i6;
   goto kopirovanie_cataloga_v_catalog3;
   }
 if(i3==zapis_dvoetochie1) {
   massiv_iera[nomer_iera1][i2]=zapis_dvoetochie2;//культура объектов
   massiv_iera[nomer_iera1][i2+1]=massiv_iera[nomer_iera1][i1+1];//первичный объект
   massiv_iera[nomer_iera1][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera1][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     i9=massiv_iera[nomer_iera1][i8];
     if(i9==catalog1)
       i9=catalog2;
     massiv_iera[nomer_iera1][i7]=i9;
     i7++;
     }
   chislo_zapiseq++;
   i2=i6;
   goto kopirovanie_cataloga_v_catalog3;
   }
 if(i3==zapis_element1) {
   massiv_iera[nomer_iera1][i2]=zapis_element2;//культура объектов
   i9=massiv_iera[nomer_iera1][i1+1];//первичный объект
   i10=i9;
   if(i9==catalog1)
     i10=catalog2;
   massiv_iera[nomer_iera1][i2+1]=i10;
   massiv_iera[nomer_iera1][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera1][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera1][i7]=massiv_iera[nomer_iera1][i8];
     i7++;
     }
   chislo_zapiseq++;
   i2=i6;
   }
kopirovanie_cataloga_v_catalog3:
 if(i5>=len)
   goto kopirovanie_cataloga_v_catalog2;
 i1=i5;
 goto kopirovanie_cataloga_v_catalog1;
kopirovanie_cataloga_v_catalog2:
 massiv_iera[nomer_iera1][2]=chislo_zapiseq;
 massiv_iera[nomer_iera1][0]=i2;
 snyatie_mecheniya_cataloga(catalog1);
 return 110100187; // 110100187 = нормальный выход
 }

// извлечение иерархии каталога из каталога иерархий или ветвей в возвращаемом значении функции
unsigned int izvlechenie_cataloga_v_iera(unsigned int catalog) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int nomer_iera2;
 unsigned int nomer_elementa1=3;//творцовый объект
 unsigned int nomer_elementa2;
 unsigned int chislo_zapiseq2;
 unsigned int zapis_dir1;
 unsigned int zapis_element1;
 unsigned int zapis_dir2;
 unsigned int zapis_element2;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 unsigned int i6;
 unsigned int i7;
 unsigned int i8;
 unsigned int len;
 nomer_iera2=blijaqshaya_pustaya_iera();
 if(nomer_iera2==0)
   return 0;// 0 = не тот номер иерархии
 if(0x1000001>nomer_iera2||nomer_iera2>=0x2000000)
   return 0;// 0 = это не иерархия
 nomer_iera2=nomer_iera2-0x1000000;
 if(0x2000001<=catalog&&catalog<=0x3000000) {
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   zapis_dir1=v_cataloge_iera_mech_catalog;
   zapis_element1=v_cataloge_iera_mech_iera;
   zapis_dir2=v_cataloge_iera_catalog;
   zapis_element2=v_cataloge_iera_iera;
   goto izvlechenie_cataloga_v_iera4;
   }
 if(0x4000001<=catalog&&catalog<=0x5000000) {
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   zapis_dir1=v_cataloge_vetveq_mech_catalog;
   zapis_element1=v_cataloge_vetveq_mech_vetv;
   zapis_dir2=v_cataloge_vetveq_catalog;
   zapis_element2=v_cataloge_vetveq_vetv;
   }
izvlechenie_cataloga_v_iera4:
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 0;// 0 = это не иерархия
 if(n1==0)
   return 0;// 0 = это не каталог иерархий или ветвей
 nomer_iera1=nomer_iera1-0x1000000;
 mechenie_cataloga(catalog);
 massiv_iera[nomer_iera2][nomer_elementa1]=massiv_iera[nomer_iera1][nomer_elementa1];
 nomer_elementa1++;//иерархия источников
 massiv_iera[nomer_iera2][nomer_elementa1]=massiv_iera[nomer_iera1][nomer_elementa1];
 nomer_elementa1++;//культура объектов
 massiv_iera[nomer_iera2][nomer_elementa1]=massiv_iera[nomer_iera1][nomer_elementa1];
 nomer_elementa1++;//старт с первой записи
 chislo_zapiseq2=0;
 i1=6;
 i2=i1;
 len=massiv_iera[nomer_iera1][0];
izvlechenie_cataloga_v_iera1:
 i3=massiv_iera[nomer_iera1][i1];
 i4=massiv_iera[nomer_iera1][i1+3];
 i5=i1+4+i4;
 i6=i2+4+i4;
 if(i3==zapis_dir1) {
   massiv_iera[nomer_iera2][i2]=zapis_dir2;//культура объектов
   massiv_iera[nomer_iera2][i2+1]=massiv_iera[nomer_iera1][i1+1];//первичный объект
   massiv_iera[nomer_iera2][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera2][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera2][i7]=massiv_iera[nomer_iera1][i8];
     i7++;
     }
   chislo_zapiseq2++;
   i2=i6;
   goto izvlechenie_cataloga_v_iera3;
   }
 if(i3==zapis_element1) {
   massiv_iera[nomer_iera2][i2]=zapis_element2;//культура объектов
   massiv_iera[nomer_iera2][i2+1]=massiv_iera[nomer_iera1][i1+1];//первичный объект
   massiv_iera[nomer_iera2][i2+2]=massiv_iera[nomer_iera1][i1+2];//цена
   i7=i2+3;
   massiv_iera[nomer_iera2][i7]=i4;//число связей
   i7++;//начало вторичных объектов
   for(i8=i1+4;i8<i5;i8++) {
     massiv_iera[nomer_iera2][i7]=massiv_iera[nomer_iera1][i8];
     i7++;
     }
   chislo_zapiseq2++;
   i2=i6;
   }
izvlechenie_cataloga_v_iera3:
 if(i5>=len)
   goto izvlechenie_cataloga_v_iera2;
 i1=i5;
 goto izvlechenie_cataloga_v_iera1;
izvlechenie_cataloga_v_iera2:
 massiv_iera[nomer_iera2][2]=chislo_zapiseq2;
 massiv_iera[nomer_iera2][1]=zanyataya_iera;
 massiv_iera[nomer_iera2][0]=i2;
 snyatie_mecheniya_cataloga(catalog);
 return 1000000+nomer_iera2;
 }

// подсчёт числа иерархий в каталоге в возвращаемом значении функции
unsigned int chislo_iera_v_cataloge(unsigned int catalog) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int i1;
 unsigned int i2;
 unsigned int f1=0;
 unsigned int f2=0;
 if(catalog>=0x2000001 && catalog<=0x3000000) {//каталог иерархий
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_iera_dvoetochie,catalog);
   if(f1==0)
     return 0;// 0 = нет данного каталога в каталоге иерархий
   }
 if(catalog>=0x4000001 && catalog<=0x5000000) {//каталог ветвей
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_vetveq_dvoetochie,catalog);
   if(f1==0)
     return 0;// 0 = нет данного каталога в каталоге ветвей
   }
 if(n1==0)
   return 0;// 0 = это не каталог иерархий или ветвей
 i1=6;
 i2=0;
 mechenie_cataloga(catalog);
 if(n1==2)
   goto chislo_iera_v_cataloge2;
chislo_iera_v_cataloge1:
 i1=poisk_kategorii_start(nomer_iera1,v_cataloge_iera_mech_iera,i1);
 if(i1!=0) {
   i2++;
   i1=poisk_sleduyscheq_zapisi(nomer_iera1,i1);
   goto chislo_iera_v_cataloge1;
   }
 snyatie_mecheniya_cataloga(catalog);
 return i2;
chislo_iera_v_cataloge2:
 i1=poisk_kategorii_start(nomer_iera1,v_cataloge_vetveq_mech_vetv,i1);
 if(i1!=0) {
   i2++;
   i1=poisk_sleduyscheq_zapisi(nomer_iera1,i1);
   goto chislo_iera_v_cataloge2;
   }
 snyatie_mecheniya_cataloga(catalog);
 return i2;
 }

// подсчёт числа иерархий и каталогов в каталоге в возвращаемом значении функции
unsigned int chislo_iera_i_catalogov_v_cataloge(unsigned int catalog) {
 unsigned int n1=0;
 unsigned int nomer_iera1;
 unsigned int i1;
 unsigned int i2;
 unsigned int f1=0;
 unsigned int f2=0;
 if(catalog>=0x2000001 && catalog<=0x3000000) {//каталог иерархий
   n1=1;
   nomer_iera1=massiv_adress_iera[1];
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_iera_dvoetochie,catalog);
   if(f1==0)
     return 0;// 0 = нет данного каталога в каталоге иерархий
   }
 if(catalog>=0x4000001 && catalog<=0x5000000) {//каталог ветвей
   n1=2;
   nomer_iera1=massiv_adress_iera[2];
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_vetveq_dvoetochie,catalog);
   if(f1==0)
     return 0;// 0 = нет данного каталога в каталоге ветвей
   }
 if(n1==0)
   return 0;// 0 = это не каталог иерархий или ветвей
 i1=6;
 i2=0;
 mechenie_cataloga(catalog);
 if(n1==2)
   goto chislo_iera_i_catalogov_v_cataloge2;
chislo_iera_i_catalogov_v_cataloge1:
 i1=poisk_kategorii_start(nomer_iera1,v_cataloge_iera_mech_iera,i1);
 if(i1!=0) {
   i2++;
   i1=poisk_sleduyscheq_zapisi(nomer_iera1,i1);
   goto chislo_iera_i_catalogov_v_cataloge1;
   }
 i1=6;
chislo_iera_i_catalogov_v_cataloge3:
 i1=poisk_kategorii_start(nomer_iera1,v_cataloge_iera_mech_catalog,i1);
 if(i1!=0) {
   i2++;
   i1=poisk_sleduyscheq_zapisi(nomer_iera1,i1);
   goto chislo_iera_i_catalogov_v_cataloge3;
   }
 snyatie_mecheniya_cataloga(catalog);
 return i2;
chislo_iera_i_catalogov_v_cataloge2:
 i1=poisk_kategorii_start(nomer_iera1,v_cataloge_vetveq_mech_vetv,i1);
 if(i1!=0) {
   i2++;
   i1=poisk_sleduyscheq_zapisi(nomer_iera1,i1);
   goto chislo_iera_i_catalogov_v_cataloge2;
   }
 i1=6;
chislo_iera_i_catalogov_v_cataloge4:
 i1=poisk_kategorii_start(nomer_iera1,v_cataloge_vetveq_mech_catalog,i1);
 if(i1!=0) {
   i2++;
   i1=poisk_sleduyscheq_zapisi(nomer_iera1,i1);
   goto chislo_iera_i_catalogov_v_cataloge4;
   }
 snyatie_mecheniya_cataloga(catalog);
 return i2;
 }

// удаление меченного
unsigned int udalenie_mechenogo(unsigned int iera) {
 unsigned int i1;
 if(0x1000001>iera||iera>=0x2000000)
   return 110100188;// 110100188 = это не иерархия
udalenie_mechenogo1:
 i1=poisk_kategorii(iera,v_cataloge_iera_mech_catalog);
 if(i1==0)
   goto udalenie_mechenogo2;
 udalenie_zapisi(iera,i1);
 goto udalenie_mechenogo1;

udalenie_mechenogo2:
 i1=poisk_kategorii(iera,v_cataloge_iera_mech_dvoetochie);
 if(i1==0)
   goto udalenie_mechenogo3;
 udalenie_zapisi(iera,i1);
 goto udalenie_mechenogo2;

udalenie_mechenogo3:
 i1=poisk_kategorii(iera,v_cataloge_iera_mech_iera);
 if(i1==0)
   goto udalenie_mechenogo4;
 udalenie_zapisi(iera,i1);
 goto udalenie_mechenogo3;

udalenie_mechenogo4:
 i1=poisk_kategorii(iera,v_cataloge_vetveq_mech_catalog);
 if(i1==0)
   goto udalenie_mechenogo5;
 udalenie_zapisi(iera,i1);
 goto udalenie_mechenogo4;

udalenie_mechenogo5:
 i1=poisk_kategorii(iera,v_cataloge_vetveq_mech_dvoetochie);
 if(i1==0)
   goto udalenie_mechenogo6;
 udalenie_zapisi(iera,i1);
 goto udalenie_mechenogo5;

udalenie_mechenogo6:
 i1=poisk_kategorii(iera,v_cataloge_vetveq_mech_vetv);
 if(i1==0)
   goto udalenie_mechenogo7;
 udalenie_zapisi(iera,i1);
 goto udalenie_mechenogo6;
udalenie_mechenogo7:
 return 110100189; // 110100189 = нормальный выход
 }

// удаление каталога из каталога иерархий или каталога ветвей
unsigned int udalenie_catalog_iz_cataloga(unsigned int catalog) {
 unsigned int nomer_iera1;
 unsigned int nomer_iera2;
 unsigned int nomer_elementa1;
 unsigned int nomer_elementa2;
 unsigned int chislo_zapiseq2;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int n1;
 unsigned int f1;
 unsigned int dlina;
 if(catalog>=0x2000001 && catalog<=0x3000000) //каталог иерархий
   n1=1;
 if(catalog>=0x4000001 && catalog<=0x5000000) //каталог ветвей
   n1=2;
 if(n1==0)
   return 110100190;// 110100190 = это не каталог иерархий или ветвей
 mechenie_cataloga(catalog);
 if(n1==1) {
   nomer_iera1=massiv_adress_iera[1];
   udalenie_mechenogo(nomer_iera1);
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_iera_dvoetochie,catalog);
   if(f1==0)
     return 110100191;// 110100191 = нет данного каталога в каталоге иерархий
   udalenie_zapisi(nomer_iera1,f1);
   i1=poisk_vtorichnogo_object(nomer_iera1,catalog);
   udalenie_vtorichnogo_object2(nomer_iera1,i1,catalog);
   }
 if(n1==2) {
   nomer_iera1=massiv_adress_iera[2];
   udalenie_mechenogo(nomer_iera1);
   f1=poisk_kategorii_i_opornogo_object(nomer_iera1,v_cataloge_vetveq_dvoetochie,catalog);
   if(f1==0) {
     return 110100192;// 110100192 = нет данного каталога в каталоге ветвей
     }
   udalenie_zapisi(nomer_iera1,f1);
   i1=poisk_vtorichnogo_object(nomer_iera1,catalog);
   udalenie_vtorichnogo_object2(nomer_iera1,i1,catalog);
   }
 return 110100193; // 110100193 = нормальный выход
 }

// инициализация каталогов иерархий и ветвей, нужное чтобы установить массив адресов и массив имен
unsigned int init_catalogi_iera(void) {
 unsigned int i1;
//инициализация массива порядка расположения значимых иерархий, связанного с именным масс
massiv_adress_iera[1]=0x1000001;//иерархия каталога иерархий
massiv_adress_iera[2]=1000002;//иерархия каталога ветвей
//инициализация именного массива
for(i1=1;i1<max_chislo_iera;i1++) {
  if(name_iera1[i1]==0)
    break;
  BZ_virtual_iera_be_name_RAM[1][i1]=name_iera1[i1];
  }
for(i1=1;i1<max_chislo_iera;i1++) {
  if(name_iera2[i1]==0)
    break;
  BZ_virtual_iera_be_name_RAM[2][i1]=name_iera2[i1];
  }
//инициализация каталога иерархий
 massiv_iera[1][0]=16;
 massiv_iera[1][1]=zanyataya_iera;
 massiv_iera[1][2]=2;
 massiv_iera[1][3]=254;//универсальная_экспертная_система_типа_1:254 - tvorec
 massiv_iera[1][4]=258;//источник_информации_типа_1:258 - istochniki
 massiv_iera[1][5]=260;//информация_типа_1:260 - kultura
 massiv_iera[1][6]=v_cataloge_iera_korn_catalog;//корневой каталог_в_каталоге_иерархий:v_cataloge_iera_korn_catalog - oblast
 massiv_iera[1][7]=0x2000001;//object1
 massiv_iera[1][8]=1;//cena
 massiv_iera[1][9]=1;//chislo
 massiv_iera[1][10]=0x2000001;//object1
 massiv_iera[1][11]=v_cataloge_iera_dvoetochie;//двоеточие_в_каталоге_иерархий:v_cataloge_iera_dvoetochie - oblast
 massiv_iera[1][12]=0x2000001;//object1
 massiv_iera[1][13]=1;//cena
 massiv_iera[1][14]=1;//chislo
 massiv_iera[1][15]=0x2000001;//object1
//инициализация каталога ветвей
 massiv_iera[2][0]=16;
 massiv_iera[2][1]=zanyataya_iera;
 massiv_iera[2][2]=2;
 massiv_iera[2][3]=254;//универсальная_экспертная_система_типа_1:254 - tvorec
 massiv_iera[2][4]=258;//источник_информации_типа_1:258 - istochniki
 massiv_iera[2][5]=260;//информация_типа_1:260 - kultura
 massiv_iera[2][6]=v_cataloge_vetveq_korn_catalog;//корневой каталог_в_каталоге_ветвей:v_cataloge_vetveq_korn_catalog
 massiv_iera[2][7]=0x4000001;//object1
 massiv_iera[2][8]=1;//cena
 massiv_iera[2][9]=1;//chislo
 massiv_iera[2][10]=0x4000001;//object1
 massiv_iera[2][11]=v_cataloge_vetveq_dvoetochie;//двоеточие_в_каталоге_ветвей:v_cataloge_vetveq_dvoetochie
 massiv_iera[2][12]=0x4000001;//object1
 massiv_iera[2][13]=1;//cena
 massiv_iera[2][14]=1;//chislo
 massiv_iera[2][15]=0x4000001;//object1
 return 110100194; // 110100194 = нормальный выход
 }

// инициализация иерархии massiv_iera[nomer_iera1]
unsigned int init_iera(unsigned int nomer_iera1, 
  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1) {
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100195;// 110100195 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 massiv_iera[nomer_iera1][0]=6;//общая длина иерархии
 massiv_iera[nomer_iera1][1]=zanyataya_iera;//формат существования иерархии
 massiv_iera[nomer_iera1][2]=0;//число записей иерархии
 massiv_iera[nomer_iera1][3]=tvorec1;
 massiv_iera[nomer_iera1][4]=istochniki1;
 massiv_iera[nomer_iera1][5]=kultura1;
 return 110100196; // 110100196 = нормальный выход
 }

// инициализация иерархии massiv_iera[nomer_iera1] массивом x1
unsigned int init_iera_massiv(unsigned int nomer_iera1, 
  unsigned int tvorec1, unsigned int istochniki1, unsigned int kultura1,
  unsigned int *x1) {
 unsigned int i1,i2,i3,i4;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100197;// 110100197 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 massiv_iera[nomer_iera1][0]=6;//общая длина иерархии
 massiv_iera[nomer_iera1][1]=zanyataya_iera;//формат существования иерархии
 massiv_iera[nomer_iera1][2]=0;//число записей иерархии
 massiv_iera[nomer_iera1][3]=tvorec1;
 massiv_iera[nomer_iera1][4]=istochniki1;
 massiv_iera[nomer_iera1][5]=kultura1;
 i2=x1[0]-1;
 for(i1=0;i1<=i2;i1++) {
   i3=6+i1;
   i4=1+i1;
   massiv_iera[nomer_iera1][i3]=x1[i4];
   }
 massiv_iera[nomer_iera1][0]=6+i2;//общая длина иерархии
 perevod_iera(nomer_iera1);
 return 110100198; // 110100198 = нормальный выход
}

// обнуление иерархии massiv_iera[nomer_iera1] до начального значения
unsigned int ochistka_iera(unsigned int nomer_iera1) {
 int i1;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100199;// 110100199 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 massiv_iera[nomer_iera1][0]=6;
 for(i1=6;i1<=max_dlina_iera;i1++)
   massiv_iera[nomer_iera1][i1]=0;
 return 110100200; // 110100200 = нормальный выход
 }

// обнуление сортировочного массива иерархии sortirovochnaya_iera[nomer_iera1]
unsigned int ochistka_sortirovochnaya_iera(unsigned int nomer_iera1) {
 int i1;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100201;// 110100201 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 for(i1=1;i1<max_chislo_zapiseq_iera;i1++) {
   sortirovochnaya_iera[nomer_iera1][i1][0]=0;
   sortirovochnaya_iera[nomer_iera1][i1][1]=0;
   sortirovochnaya_iera[nomer_iera1][i1][2]=0;
   sortirovochnaya_iera[nomer_iera1][i1][3]=0;
   sortirovochnaya_iera[nomer_iera1][i1][4]=0;
   sortirovochnaya_iera[nomer_iera1][i1][5]=0;
   sortirovochnaya_iera[nomer_iera1][i1][6]=0;
   }
 return 110100202; // 110100202 = нормальный выход
 }

// сортировка иерархии
// возвращаемое значение ошибки:
// 110100203 = это не иерархия
// 110100204 = пустая иерархия
// 110100205 = есть первичный объект без связей
// 110100206 = всё впорядке,всё рассортированно
// 110100207 = невозможно сортировать по вторичным объектам
unsigned int sortirovka_iera(unsigned int nomer_iera1) {
unsigned int nomer_zapisi1;
unsigned int nomer_zapisi2;
 unsigned int ao1,ao2,ao3,ao4;
 unsigned int o1,o2;
unsigned int i1,i2,i3;
unsigned int chislo_object1;
unsigned int chislo_zapiseq1;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100203;// 110100203 = это не иерархия;
 nomer_iera1=nomer_iera1-0x1000000;
 chislo_zapiseq1=massiv_iera[nomer_iera1][2];
 if(chislo_zapiseq1==0) return 110100204;// 110100204 = некорректное чисо записей
 if(chislo_zapiseq1==1) {
//сортировка связных объектов еденичной записи
   nomer_zapisi1=1;
sortirovka_iera70:
   if(nomer_zapisi1>chislo_zapiseq1) goto sortirovka_iera100;
   chislo_object1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
   if(chislo_object1==1) {
     nomer_zapisi1++;
     goto sortirovka_iera70;
     }
   if(chislo_object1==0) {
     nomer_zapisi1++;
     goto sortirovka_iera70;
     }
   i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
   i2=i1+4+chislo_object1;
   ao1=i1+4;
   ao2=i1+5;
   ao3=ao1;
   ao4=ao2;
sortirovka_iera80:
   if(ao2>=i2) goto sortirovka_iera90;
   o1=massiv_iera[nomer_iera1][ao1];
   o2=massiv_iera[nomer_iera1][ao2];
   if(o1>o2) {
     massiv_iera[nomer_iera1][ao1]=o2;
     massiv_iera[nomer_iera1][ao2]=o1;
     if(ao1!=ao3&&ao2!=ao4) {
       ao1--;
       ao2--;
       }
       else {
         ao1++;
         ao2++;
         }
     goto sortirovka_iera80;
     }
   ao1++;
   ao2++;
   goto sortirovka_iera80;
sortirovka_iera90:
   nomer_zapisi1++;
   goto sortirovka_iera70;
sortirovka_iera100:
   return 110100206; // 110100206 = нормальный выход
   }
//сортировка по областям
 nomer_zapisi1=1;
 nomer_zapisi2=2;
sortirovka_iera1:
 if(nomer_zapisi2>chislo_zapiseq1) goto sortirovka_iera2;
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2];
 if(i1>i2) {
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2]=i2;
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2]=i1;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6]=i3;
   if(nomer_zapisi1!=1&&nomer_zapisi2!=2) {
     nomer_zapisi1--;
     nomer_zapisi2--;
     }
     else {
       nomer_zapisi1++;
       nomer_zapisi2++;
       }
   goto sortirovka_iera1;
   }
 nomer_zapisi1++;
 nomer_zapisi2++;
 goto sortirovka_iera1;
sortirovka_iera2:
//сортировка по опорным объектам
 nomer_zapisi1=1;
 nomer_zapisi2=2;
sortirovka_iera3:
 if(nomer_zapisi2>chislo_zapiseq1) goto sortirovka_iera4;
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2];
 if(i1!=i2) {
   nomer_zapisi1++;
   nomer_zapisi2++;
   goto sortirovka_iera3;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
 if(i1>i2) {
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]=i2;
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3]=i1;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6]=i3;
   if(nomer_zapisi1!=1&&nomer_zapisi2!=2) {
     nomer_zapisi1--;
     nomer_zapisi2--;
     }
     else {
       nomer_zapisi1++;
       nomer_zapisi2++;
       }
   goto sortirovka_iera3;
   }
 nomer_zapisi1++;
 nomer_zapisi2++;
 goto sortirovka_iera3;
sortirovka_iera4:
//сортировка по оценкам
 nomer_zapisi1=1;
 nomer_zapisi2=2;
sortirovka_iera5:
 if(nomer_zapisi2>chislo_zapiseq1) goto sortirovka_iera6;
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2];
 if(i1!=i2) {
   nomer_zapisi1++;
   nomer_zapisi2++;
   goto sortirovka_iera5;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
 if(i1!=i2) {
   nomer_zapisi1++;
   nomer_zapisi2++;
   goto sortirovka_iera5;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
 if(i1>i2) {
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4]=i2;
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4]=i1;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6]=i3;
   if(nomer_zapisi1!=1&&nomer_zapisi2!=2) {
     nomer_zapisi1--;
     nomer_zapisi2--;
     }
     else {
       nomer_zapisi1++;
       nomer_zapisi2++;
       }
   goto sortirovka_iera5;
   }
 nomer_zapisi1++;
 nomer_zapisi2++;
 goto sortirovka_iera5;
sortirovka_iera6:
//сортировка связных объектов
 nomer_zapisi1=1;
sortirovka_iera7:
 if(nomer_zapisi1>chislo_zapiseq1) goto sortirovka_iera10;
 chislo_object1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
 if(chislo_object1==1) {
   nomer_zapisi1++;
   goto sortirovka_iera7;
   }
 if(chislo_object1==0) {
   nomer_zapisi1++;
   goto sortirovka_iera7;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
 i2=i1+4+chislo_object1;
 ao1=i1+4;
 ao2=i1+5;
 ao3=ao1;
 ao4=ao2;
sortirovka_iera8:
 if(ao2>=i2) goto sortirovka_iera9;
 o1=massiv_iera[nomer_iera1][ao1];
 o2=massiv_iera[nomer_iera1][ao2];
 if(o1>o2) {
   massiv_iera[nomer_iera1][ao1]=o2;
   massiv_iera[nomer_iera1][ao2]=o1;
   if(ao1!=ao3&&ao2!=ao4) {
     ao1--;
     ao2--;
     }
     else {
       ao1++;
       ao2++;
       }
   goto sortirovka_iera8;
   }
 ao1++;
 ao2++;
 goto sortirovka_iera8;
sortirovka_iera9:
 nomer_zapisi1++;
 goto sortirovka_iera7;
sortirovka_iera10:
//сортировка по вторичным объектам
 nomer_zapisi1=1;
 nomer_zapisi2=2;
sortirovka_iera11:
 if(nomer_zapisi2>chislo_zapiseq1) goto sortirovka_iera12;
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2];
 if(i1!=i2) {
   nomer_zapisi1++;
   nomer_zapisi2++;
   goto sortirovka_iera11;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
 if(i1!=i2) {
   nomer_zapisi1++;
   nomer_zapisi2++;
   goto sortirovka_iera11;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
 if(i1!=i2) {
   nomer_zapisi1++;
   nomer_zapisi2++;
   goto sortirovka_iera11;
   }
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
 if(i1==0)
   return 110100205;// 110100205 = невозможно сортировать по вторичным объектам
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
 if(i2==0)
   return 110100207;// 110100207 = невозможно сортировать по вторичным объектам
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
 i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
 ao1=i1+4;
 ao2=i2+4;
 o1=massiv_iera[nomer_iera1][ao1];
 o2=massiv_iera[nomer_iera1][ao2];
 if(o1>o2) {
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][0]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][0]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][1]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][1]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]=i3;
   i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
   sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6]=i3;
   if(nomer_zapisi1!=1&&nomer_zapisi2!=2) {
     nomer_zapisi1--;
     nomer_zapisi2--;
     }
     else {
       nomer_zapisi1++;
       nomer_zapisi2++;
       }
   goto sortirovka_iera11;
   }
 nomer_zapisi1++;
 nomer_zapisi2++;
 goto sortirovka_iera11;
sortirovka_iera12:
 return 110100206;// 110100206 = всё впорядке,всё рассортированно
 }

// в иерархию добавляется запись - опорный объект с массивом связей
// x1 - область объектов
// x2 - опорный объект
// x3 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// x4 - массив объектов связанных с опорным объектом
unsigned int dozapis_end_iera_massiv(unsigned int nomer_iera,
  unsigned int x1, unsigned int x2, unsigned int x3, unsigned int *x4) {
 unsigned int nomer_elementa;
 unsigned int chislo_zapiseq1;
 unsigned int i1,i2;
 unsigned int len1;
 unsigned int len2;
 unsigned int istochnik1;
 unsigned int dlina;
 if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
   return 110100208;// 110100208 = это не иерархия
 nomer_iera=nomer_iera-0x1000000;
 nomer_elementa=massiv_iera[nomer_iera][0];
 len1=x4[0];
 if(len1<2)
   return 110100209;// 110100209 = некорректный массив
 len2=nomer_elementa+3+len1;
 if(len2>max_dlina_iera)
   return 110100427;// 110100427 = выход за длину иерархии
 massiv_iera[nomer_iera][nomer_elementa]=x1;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x2;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x3;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=len1-1;
 nomer_elementa++;
 for(i1=1;i1<len1;i1++) {
   massiv_iera[nomer_iera][nomer_elementa]=x4[i1];
   nomer_elementa++;
   }
 chislo_zapiseq1=massiv_iera[nomer_iera][2];
 chislo_zapiseq1++;
 massiv_iera[nomer_iera][2]=chislo_zapiseq1;
 massiv_iera[nomer_iera][0]=nomer_elementa;
 return 110100210; // 110100210 = нормальный выход
 }

// в иерархию добавляется запись - опорный объект с одной связью
// x1 - область объектов
// x2 - опорный объект
// x3 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
// x4 - номер объекта связанного с опорным объектом
unsigned int dozapis_end_iera_object(unsigned int nomer_iera,
  unsigned int x1, unsigned int x2, unsigned int x3, unsigned int x4) {
 unsigned int chislo_zapiseq1;
 unsigned int nomer_elementa;
 unsigned int len;
 if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
   return 110100211;// 110100211 = это не иерархия
 nomer_iera=nomer_iera-0x1000000;
 nomer_elementa=massiv_iera[nomer_iera][0];
 len=nomer_elementa+5;
 if(len>max_dlina_iera)
   return 110100212;// 110100212 = выход за длину иерархии
 massiv_iera[nomer_iera][nomer_elementa]=x1;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x2;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x3;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=1;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x4;
 nomer_elementa++;
 massiv_iera[nomer_iera][0]=nomer_elementa;
 chislo_zapiseq1=massiv_iera[nomer_iera][2];
 chislo_zapiseq1++;
 massiv_iera[nomer_iera][2]=chislo_zapiseq1;
 return 110100213; // 110100213 = нормальный выход
 }

// в иерархию добавляется запись - опорный объект с нулем вторичных связей
// x1 - область объектов
// x2 - опорный объект
// x3 - параметры оценочной стоимости данной связи, в случае отсутствия сортировки = 0
unsigned int dozapis_end_iera_object_zero(unsigned int nomer_iera,
  unsigned int x1, unsigned int x2, unsigned int x3) {
 unsigned int chislo_zapiseq1;
 unsigned int nomer_elementa;
 unsigned int len;
 if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
   return 110100214;// 110100214 = это не иерархия
 nomer_iera=nomer_iera-0x1000000;
 nomer_elementa=massiv_iera[nomer_iera][0];
 len=nomer_elementa+4;
 if(len>max_dlina_iera)
   return 110100215;// 110100215 = выход за длину иерархии
 massiv_iera[nomer_iera][nomer_elementa]=x1;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x2;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=x3;
 nomer_elementa++;
 massiv_iera[nomer_iera][nomer_elementa]=0;
 nomer_elementa++;
 massiv_iera[nomer_iera][0]=nomer_elementa;
 chislo_zapiseq1=massiv_iera[nomer_iera][2];
 chislo_zapiseq1++;
 massiv_iera[nomer_iera][2]=chislo_zapiseq1;
 return 110100216; // 110100216 = нормальный выход
 }

// добавление записи в каталог иерархий или ветвей
unsigned int dozapis_v_catalog(unsigned int catalog, unsigned int object) {
 unsigned int nomer_iera;
 unsigned int n1=0;
 unsigned int f1=0;
 if(catalog>=0x2000001 && catalog<=0x3000000) { //каталог иерархий
   n1=1;
   nomer_iera=massiv_adress_iera[1];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 110100219;// 110100219 = это не иерархия
   }
 if(catalog>=0x4000001 && catalog<=0x5000000) { //каталог ветвей
   n1=2;
   nomer_iera=massiv_adress_iera[2];
   if(nomer_iera<0x1000000||nomer_iera>=0x2000000)
     return 110100220;// 110100220 = это не иерархия
   }
 if(n1==0)
   return 110100221;// 110100221 = это не каталог каталога иерархий или ветвей
 if(object>=0x2000001 && object<=0x3000000) {
   f1=poisk_kategorii_i_opornogo_object(nomer_iera,v_cataloge_iera_dvoetochie,catalog);
   if(f1==0)
     return 110100222;// 110100222 = нет данного каталога в каталоге иерархий
   dozapis_end_iera_object(nomer_iera,v_cataloge_iera_catalog,catalog,1,object);
   dozapis_end_iera_object(nomer_iera,v_cataloge_iera_dvoetochie,object,1,catalog);
    return 110100223; // 110100223 = нормальный выход
   }
 if(object>=0x4000001 && object<=0x5000000) {
   f1=poisk_kategorii_i_opornogo_object(nomer_iera,v_cataloge_vetveq_dvoetochie,catalog);
   if(f1==0) {
     return 110100224;// 110100224 = нет данного каталога в каталоге ветвей
     }
   dozapis_end_iera_object(nomer_iera,v_cataloge_vetveq_catalog,catalog,1,object);
   dozapis_end_iera_object(nomer_iera,v_cataloge_vetveq_dvoetochie,object,1,catalog);
   return 110100225; // 110100225 = нормальный выход
   }
 if(n1==1) {
   f1=poisk_kategorii_i_opornogo_object(nomer_iera,v_cataloge_iera_dvoetochie,catalog);
   if(f1==0)
     return 110100226;// 110100226 = нет данного каталога в каталоге иерархий
   dozapis_end_iera_object(nomer_iera,v_cataloge_iera_iera,catalog,1,object);
   return 110100227; // 110100227 = нормальный выход
   }
 if(n1==2) {
   f1=poisk_kategorii_i_opornogo_object(nomer_iera,v_cataloge_vetveq_dvoetochie,catalog);
   if(f1==0)
     return 110100228;// 110100228 = нет данного каталога в каталоге ветвей
   dozapis_end_iera_object(nomer_iera,v_cataloge_vetveq_vetv,catalog,1,object);
   return 110100229; // 110100229 = нормальный выход
   }
 }

// определение точного включения 2й иерархии в 1ю без всяких хвостов, первая больше
// вывод кода если иерархии совпадают возвращает значение =1,  если несовпадают =0
unsigned int tochnoe_vklychenie_iera(unsigned int nomer_iera1, unsigned int nomer_iera2) {
 unsigned int result1;
 unsigned int chislo_zapiseq1;
 unsigned int chislo_zapiseq2;
 unsigned int nomer_zapisi1;
 unsigned int nomer_zapisi2;
 unsigned int i=0;
 unsigned int i1,i2,i3;
 unsigned int metka[1000000];
 unsigned int ao1,ao2;
 unsigned int aon1,aon2;
 unsigned int aok1,aok2;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100230;// 110100230 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
   return 110100231;// 110100231 = это не иерархия
 nomer_iera2=nomer_iera2-0x1000000;
 chislo_zapiseq2=massiv_iera[nomer_iera2][2];
 if(chislo_zapiseq2==0) return 110100232;// 110100232 = иерархия 2 включает иерархию 1 в себе
 chislo_zapiseq1=massiv_iera[nomer_iera1][2];
 nomer_zapisi1=1;
 nomer_zapisi2=1;
 for(i2=1;i2<=chislo_zapiseq2;i2++) metka[i2]=sortirovochnaya_iera[nomer_iera2][i2][5];
 for(i2=1;i2<=chislo_zapiseq2;i2++) {
   aon2=4+sortirovochnaya_iera[nomer_iera2][i2][6];
   aok2=aon2+sortirovochnaya_iera[nomer_iera2][i2][5];
   for(ao2=aon2;ao2<aok2;ao2++) {
     for(i1=1;i1<=chislo_zapiseq1;i1++) {
       if(sortirovochnaya_iera[nomer_iera1][i1][2]!=sortirovochnaya_iera[nomer_iera2][i2][2]) goto tochnoe_vklychenie_iera1;
       if(sortirovochnaya_iera[nomer_iera1][i1][3]!=sortirovochnaya_iera[nomer_iera2][i2][3]) goto tochnoe_vklychenie_iera1;
       aon1=4+sortirovochnaya_iera[nomer_iera1][i1][6];
       aok1=aon1+sortirovochnaya_iera[nomer_iera1][i1][5];
       for(ao1=aon1;ao1<aok1;ao1++) {
         if(massiv_iera[nomer_iera1][ao1]==massiv_iera[nomer_iera2][ao2]) {
	   metka[i2]--;
	   goto tochnoe_vklychenie_iera2;
           }
	 }
tochnoe_vklychenie_iera1:
       ;
       }
tochnoe_vklychenie_iera2:
     ;
     }
   }
 for(i2=1;i2<=chislo_zapiseq2;i2++) if(metka[i2]!=0) return 110100233;// 110100233 = иерархия 2 не включает иерархию 1 в себе
 return 110100234;// 110100234 = иерархия 2 включает иерархию 1 в себе
 }

// удаление избыточности из иерархии 1, всё оставшееся переносится в иерархию 2
unsigned int udalenie_lishnego_iera(unsigned int nomer_iera2, unsigned int nomer_iera1) {
 unsigned int tvorec1;
 unsigned int istochniki1;
 unsigned int kultura1;
 unsigned int chislo_zapiseq1;
 unsigned int chislo_zapiseq2;
 unsigned int nomer_zapisi1,nomer_zapisi2;
 unsigned int nomer_elementa2;
 unsigned int o1,o2;
 unsigned int ao1,ao2;
 unsigned int aon1,aon2;
 unsigned int aok1,aok2;
 unsigned int aks2;
 unsigned int ks2;
 unsigned int flag_zagolovka;
 unsigned int flag_pervogo_elementa;
 unsigned int len;
 perevod_iera(nomer_iera1);
 sortirovka_iera(nomer_iera1);
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100235;// 110100235 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 tvorec1=massiv_iera[nomer_iera1][3];
 istochniki1=massiv_iera[nomer_iera1][4];
 kultura1=massiv_iera[nomer_iera1][5];
 init_iera(nomer_iera2,tvorec1,istochniki1,kultura1);
 if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
   return 110100236;// 110100236 = это не иерархия
 nomer_iera2=nomer_iera2-0x1000000;
 chislo_zapiseq1=massiv_iera[nomer_iera1][2];
 if(chislo_zapiseq1==0) return 110100237;// 110100237 = пустая иерархия
 nomer_zapisi1=1;
 nomer_zapisi2=1;
 nomer_elementa2=6;
 chislo_zapiseq2=0;
 flag_pervogo_elementa=0;
udalenie_lishnego_iera8:
 len=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
 if(len==0) {
   nomer_zapisi2++;
   if(nomer_zapisi2>chislo_zapiseq1) return 110100238;// 110100238 = иерархия очищена от избыточности
   goto udalenie_lishnego_iera8;
   }
 flag_zagolovka=1;
 aon2=4+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
 ao2=aon2;
 aok2=aon2+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];

udalenie_lishnego_iera7:
 if(nomer_zapisi1==nomer_zapisi2) {
   flag_pervogo_elementa=1;
   goto udalenie_lishnego_iera9;
   }
 if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2]!=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2]) {
   nomer_zapisi1++;
   goto udalenie_lishnego_iera7;
   }
 if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]!=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3]) {
   nomer_zapisi1++;
   goto udalenie_lishnego_iera7;
   }
 if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][4]!=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4]) {
   nomer_zapisi1++;
   goto udalenie_lishnego_iera7;
   }
 if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]==0) 
   goto udalenie_lishnego_iera5;
udalenie_lishnego_iera9:
 aon1=4+sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
 ao1=aon1;
 len=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
 aok1=aon1+len;
 if(flag_pervogo_elementa==1&&len==1)
    goto udalenie_lishnego_iera1;

udalenie_lishnego_iera4:
 if(massiv_iera[nomer_iera1][ao1]==massiv_iera[nomer_iera1][ao2])
   goto udalenie_lishnego_iera3;
 ao1++;
 if(ao1>=aok1) {
   nomer_zapisi1++;
   goto udalenie_lishnego_iera7;
   }
 if(ao1<ao2) goto udalenie_lishnego_iera4;

udalenie_lishnego_iera1:
 if(flag_zagolovka==1) {
   flag_zagolovka=0;
   chislo_zapiseq2++;
   massiv_iera[nomer_iera2][nomer_elementa2]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2];
   nomer_elementa2++; // копирование области
   massiv_iera[nomer_iera2][nomer_elementa2]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
   nomer_elementa2++; // копирование опорного элемента
   massiv_iera[nomer_iera2][nomer_elementa2]=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
   nomer_elementa2++; // копирование сортировки
   aks2=nomer_elementa2;// копирование адреса количества связных объектов
   ks2=0;
   nomer_elementa2++;
   if(flag_pervogo_elementa==1) {
     flag_pervogo_elementa=0;
     if(len!=1) {
       massiv_iera[nomer_iera2][nomer_elementa2]=massiv_iera[nomer_iera1][aon2];
       nomer_elementa2++;
       ks2++;
       }
     }
   }
 massiv_iera[nomer_iera2][nomer_elementa2]=massiv_iera[nomer_iera1][ao2];
 nomer_elementa2++;
 ks2++;
 massiv_iera[nomer_iera2][aks2]=ks2;
 nomer_zapisi1=1;
udalenie_lishnego_iera3:
 ao2++;
 if(ao2>=aok2) {
   goto udalenie_lishnego_iera5;
   }
 goto udalenie_lishnego_iera7;

udalenie_lishnego_iera5:
 nomer_zapisi2++;
 if(nomer_zapisi2>chislo_zapiseq1)
   goto udalenie_lishnego_iera6;
 flag_zagolovka=1;
// chislo_zapiseq2++;
 nomer_zapisi1=1;
 aon2=4+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
 ao2=aon2;
 aok2=aon2+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5];
 goto udalenie_lishnego_iera7;
 
udalenie_lishnego_iera6:
 massiv_iera[nomer_iera2][0]=nomer_elementa2;
 massiv_iera[nomer_iera2][2]=chislo_zapiseq2;
 nomer_iera2=nomer_iera2+0x1000000;
 perevod_iera(nomer_iera2);
 sortirovka_iera(nomer_iera2);
 massiv_iera[nomer_iera1][1]=svobodnaya_iera;
 return 110100239;// 110100239 = иерархия очищена от избыточности
 }

// удаление хоста из иерархии
unsigned int udalenie_host_iz_iera(unsigned int iera2,unsigned int iera1,
  unsigned int kategoriya,unsigned int host) {
 unsigned int chislo_zapiseq1;
 unsigned int tvorec1;
 unsigned int istochniki1;
 unsigned int kultura1;
 unsigned int len;
 unsigned int i1;
 unsigned int i2;
 unsigned int i3;
 unsigned int i4;
 unsigned int i5;
 if(iera1<0x1000000||iera1>=0x2000000)
   return 110100240;// 110100240 = это не иерархия
 if(iera2<0x1000000||iera2>=0x2000000)
   return 110100241;// 110100241 = это не иерархия
 iera1=iera1-0x1000000;
 iera2=iera2-0x1000000;
 len=massiv_iera[iera1][0];
 massiv_iera[iera2][1]=zanyataya_iera;
 chislo_zapiseq1=massiv_iera[iera1][2];
 tvorec1=massiv_iera[iera1][3];
 massiv_iera[iera2][3]=tvorec1;
 istochniki1=massiv_iera[iera1][4];
 massiv_iera[iera2][4]=istochniki1;
 kultura1=massiv_iera[iera1][5];
 massiv_iera[iera2][5]=kultura1;
 i1=poisk_kategorii_i_opornogo_object(iera1+0x1000000,kategoriya,host);
 if(i1==0)
   return 110100242;// 110100242 = нет объекта
 i2=poisk_sleduyscheq_zapisi(iera1+0x1000000,i1);
 i5=i2-i1; 
 for(i3=6;i3<i1;i3++)
   massiv_iera[iera2][i3]=massiv_iera[iera1][i3];
 for(i4=i2;i4<len;i4++) {
   massiv_iera[iera2][i3]=massiv_iera[iera1][i4];
   i3++;
   }
 massiv_iera[iera2][2]=chislo_zapiseq1-1;
 massiv_iera[iera2][0]=len-i5;
 return 110100243;// 110100243 = нормальный выход
 }

// уплотнение каталога иерархий
unsigned int uplotnenie_cataloga_iera(void) {
 unsigned int nomer_iera1;
 unsigned int i1;
 unsigned int i2;
 nomer_iera1=massiv_adress_iera[1];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100244;// 110100244 = это не иерархия
 if(massiv_iera[nomer_iera1-0x1000000][0]<7)
   return 110100245;// 110100245 = это пустая иерархия
 i1=blijaqshaya_pustaya_iera();
 if(i1==0)
   return 110100246;// 110100246 = нет требуемой пустой иерархии
 i2=udalenie_lishnego_iera(i1,nomer_iera1);
 if(i2!=0)
   return 110100247;// 110100247 = ошибка уплотнения каталога
 massiv_adress_iera[1]=i1;
 return 110100248;// 110100248 = нормальный выход
 }

// уплотнение каталога ветвей
unsigned int uplotnenie_cataloga_vetveq(void) {
 unsigned int nomer_iera1;
 unsigned int i1;
 unsigned int i2;
 nomer_iera1=massiv_adress_iera[2];
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100249;// 110100249 = это не иерархия
 if(massiv_iera[nomer_iera1-0x1000000][0]<7)
   return 110100250;// 110100250 = это пустая иерархия
 i1=blijaqshaya_pustaya_iera();
 if(i1==0)
   return 110100251;// 110100251 = нет требуемой пустой иерархии
 i2=udalenie_lishnego_iera(i1,nomer_iera1);
 if(i2!=0)
   return 110100252;// 110100252 = ошибка уплотнения каталога
 massiv_adress_iera[2]=i1;
 return 110100253;// 110100253 = нормальный выход
 }

// склеивание двух иерархий, в результате третья иерархия
unsigned int skleivanie_iera(unsigned int nomer_iera3
  , unsigned int nomer_iera1, unsigned int nomer_iera2) {
 unsigned int nomer_iera4;
 unsigned int nomer_zapisi1,nomer_zapisi2,chislo_object;
 nomer_iera4=blijaqshaya_pustaya_iera();
 nomer_iera4=nomer_iera4-0x1000000;
 if(nomer_iera4==0)
   return 110100254;// 110100254 = нет свободных иерархий
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100255;// 110100255 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
   return 110100256;// 110100256 = это не иерархия
 nomer_iera2=nomer_iera2-0x1000000;
 chislo_object=massiv_iera[nomer_iera1][0];
 for(nomer_zapisi1=2;nomer_zapisi1<chislo_object;nomer_zapisi1++) {
   massiv_iera[nomer_iera4][nomer_zapisi1]=massiv_iera[nomer_iera1][nomer_zapisi1];
   }
 chislo_object=massiv_iera[nomer_iera2][0];
 for(nomer_zapisi2=2;nomer_zapisi2<chislo_object;nomer_zapisi2++) {
   massiv_iera[nomer_iera4][nomer_zapisi1]=massiv_iera[nomer_iera2][nomer_zapisi2];
   nomer_zapisi1++;
   }
 massiv_iera[nomer_iera4][0]=nomer_zapisi1;
 massiv_iera[nomer_iera4][1]=zanyataya_iera;
 massiv_iera[nomer_iera4][2]=massiv_iera[nomer_iera1][2]+massiv_iera[nomer_iera2][2];
 massiv_iera[nomer_iera4][3]=massiv_iera[nomer_iera1][3];
 massiv_iera[nomer_iera4][4]=massiv_iera[nomer_iera1][4];
 massiv_iera[nomer_iera4][5]=massiv_iera[nomer_iera1][5];
 nomer_iera4=nomer_iera4+0x1000000;
 perevod_iera(nomer_iera4);
 udalenie_lishnego_iera(nomer_iera3,nomer_iera4);
 massiv_iera[nomer_iera4-0x1000000][1]=svobodnaya_iera;
 return 110100257;// 110100257 = нормальный выход
 }

// склеивание двух иерархий, в результате третья иерархия, записываемая в первую
unsigned int skleivanie_end_iera(unsigned int nomer_iera2, unsigned int nomer_iera1) {
 unsigned int nomer_zapisi1,nomer_zapisi2,chislo_object;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100258;// 110100258 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 if(nomer_iera2<0x1000000||nomer_iera2>=0x2000000)
   return 110100259;// 110100259 = это не иерархия
 nomer_iera2=nomer_iera2-0x1000000;
 nomer_zapisi2=massiv_iera[nomer_iera2][0];
 chislo_object=massiv_iera[nomer_iera1][0];
 for(nomer_zapisi1=2;nomer_zapisi1<chislo_object;nomer_zapisi1++) {
   massiv_iera[nomer_iera2][nomer_zapisi2]=massiv_iera[nomer_iera1][nomer_zapisi1];
   nomer_zapisi2++;
   }
 massiv_iera[nomer_iera2][0]=nomer_zapisi2;
 massiv_iera[nomer_iera2][1]=zanyataya_iera;
 massiv_iera[nomer_iera2][2]=massiv_iera[nomer_iera2][2]+massiv_iera[nomer_iera1][2];
 massiv_iera[nomer_iera2][3]=massiv_iera[nomer_iera1][3];
 massiv_iera[nomer_iera2][4]=massiv_iera[nomer_iera1][4];
 massiv_iera[nomer_iera2][5]=massiv_iera[nomer_iera1][5];
 nomer_iera2=nomer_iera2+0x1000000;
 return 110100260;// 110100260 = нормальный выход
 }

// инициализация стартовой ветви дерева из иерархии по заданной области
unsigned int init_vetvi_iera(unsigned int vetv1, unsigned int nomer_iera1,
  unsigned int oblast_object) {
 unsigned int len;
 unsigned int chislo_zapiseq1;
 unsigned int nomer_elementa1;
 unsigned int tvorec1;
 unsigned int istochniki1;
 unsigned int kultura1;
 unsigned int nomer_zapisi1,nomer_zapisi2;
 unsigned int flag_pervoq_zapisi=1;
 unsigned int o1,o2,o3;
 unsigned int ao2;
 unsigned int a_ocenki;
 unsigned int ocenka;
 unsigned int a_kolichestva_object;
 unsigned int kolichestvo_object;
 unsigned int flag_object=0;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100261;// 110100261 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 if(vetv1<0x3000001||vetv1>0x4000000) {
   return 110100262;// 110100262 = это не ветвь
   }
 vetv1=vetv1-0x3000000;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq1=massiv_iera[nomer_iera1][2];
 if(len<=6||chislo_zapiseq1<1) return 110100263;// 110100263 = нет записей в иерархии
 perevod_iera(nomer_iera1);
 sortirovka_iera(nomer_iera1);
 nomer_elementa1=1;
 massiv_vetveq[vetv1][nomer_elementa1]=1;
 nomer_elementa1++;
 tvorec1=massiv_iera[nomer_iera1][3];
 istochniki1=massiv_iera[nomer_iera1][4];
 kultura1=massiv_iera[nomer_iera1][5];
 massiv_vetveq[vetv1][nomer_elementa1]=tvorec1;
 nomer_elementa1++;
 massiv_vetveq[vetv1][nomer_elementa1]=istochniki1;
 nomer_elementa1++;
 massiv_vetveq[vetv1][nomer_elementa1]=kultura1;
 nomer_elementa1++;
 massiv_vetveq[vetv1][nomer_elementa1]=oblast_object;
 nomer_elementa1++;
 a_ocenki=nomer_elementa1;
 ocenka=0;
 nomer_elementa1++;
 a_kolichestva_object=nomer_elementa1;
 kolichestvo_object=0;
 nomer_elementa1++;
 nomer_zapisi1=1;
 nomer_zapisi2=1;
init_oblasti_iera2:
 if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][2]!=oblast_object) 
   goto init_oblasti_iera1;
 if(flag_pervoq_zapisi==1) {
   if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]==0)
     goto init_oblasti_iera1;
   flag_pervoq_zapisi=0;
   o1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
   ao2=4+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
   o2=massiv_iera[nomer_iera1][ao2];
   massiv_vetveq[vetv1][nomer_elementa1]=o1;
   nomer_elementa1++;
   ocenka=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
   kolichestvo_object++;
   flag_object=1;
   }
 o3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][3];
 if(o3!=o2)
   goto init_oblasti_iera3;
 if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][5]==0)
   goto init_oblasti_iera3;
 massiv_vetveq[vetv1][nomer_elementa1]=o3;
 nomer_elementa1++;
 o1=o3;
 ao2=4+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][6];
 o2=massiv_iera[nomer_iera1][ao2];
 ocenka=ocenka+sortirovochnaya_iera[nomer_iera1][nomer_zapisi2][4];
 kolichestvo_object++;
init_oblasti_iera3:
 nomer_zapisi1++;
 if(nomer_zapisi1<nomer_zapisi2)
   goto init_oblasti_iera2;
init_oblasti_iera1:
 nomer_zapisi2++;
 if(nomer_zapisi2<chislo_zapiseq1) {
   nomer_zapisi1=1;
   goto init_oblasti_iera2;
   }
 massiv_vetveq[vetv1][nomer_elementa1]=o2;
 massiv_vetveq[vetv1][a_ocenki]=ocenka;
 massiv_vetveq[vetv1][a_kolichestva_object]=kolichestvo_object+1;
 massiv_vetveq[vetv1][0]=9+kolichestvo_object;
 if(flag_object==0) {
   massiv_vetveq[vetv1][a_kolichestva_object]=0;
   massiv_vetveq[vetv1][0]=8;
   }
 return 110100264;// 110100264 = нормальный выход
 }

// следующая ветвь иерархии
// 110100265 = это не иерархия
// 110100266 = это не ветвь
// 110100267 = это не ветвь

// 110100268 = нет записей в иерархии
// 110100269 = пустая первичная ветвь
// 110100270 = нет полного соответствия ветви в иерархии
// 110100271 = дерево кончилось, все ветви перебраны

unsigned int next_vetv_iera(unsigned int vetv2, unsigned int nomer_iera1,
  unsigned int vetv1) {
 unsigned int len;
 unsigned int chislo_zapiseq_iera;
 unsigned int message_exit;
 unsigned int nomer_elementa_vetvi1;
 unsigned int nomer_elementa_vetvi2;
 unsigned int oblast_object;
 unsigned int nomer_elementa_iera1;
 unsigned int a_ocenki;
 unsigned int ocenka;
 unsigned int a_kolichestva_object2;
 unsigned int kolichestvo_object1,kolichestvo_object2;
 unsigned int nomer_object1,nomer_object2;
 unsigned int a_nachala;
 unsigned int nomer_zapisi1;// запись в сортировочном массиве
 unsigned int nomer_zapisi2;// запись в четверном массиве
 unsigned int chislo_zapiseq1;
 unsigned int chislo_zapiseq2;
 unsigned int now_object;
 unsigned int i1,i2,i3,i4;
 unsigned int next_object;
 if(nomer_iera1<0x1000000||nomer_iera1>=0x2000000)
   return 110100265;// 110100265 = это не иерархия
 nomer_iera1=nomer_iera1-0x1000000;
 if(vetv1<0x3000001||vetv1>0x4000000)
   return 110100266;// 110100266 = это не ветвь
 vetv1=vetv1-0x3000000;
 if(vetv2<0x3000001||vetv2>0x4000000)
   return 110100267;// 110100267 = это не ветвь
 vetv2=vetv2-0x3000000;
 len=massiv_iera[nomer_iera1][0];
 chislo_zapiseq_iera=massiv_iera[nomer_iera1][2];
 if(len<=6||chislo_zapiseq_iera<1) {
   message_exit=110100268;// 110100268 = нет записей в иерархии
   goto next_vetv_iera_6;
   }
 nomer_elementa_vetvi2=0;
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=8;
 nomer_elementa_vetvi2++;
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=1;
 nomer_elementa_vetvi2++;
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=massiv_vetveq[vetv1][nomer_elementa_vetvi2];//tvorec
 nomer_elementa_vetvi2++;
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=massiv_vetveq[vetv1][nomer_elementa_vetvi2];//istochniki1
 nomer_elementa_vetvi2++;
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=massiv_vetveq[vetv1][nomer_elementa_vetvi2];//kultura
 nomer_elementa_vetvi2++;
 oblast_object=massiv_vetveq[vetv1][nomer_elementa_vetvi2];
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=oblast_object;//oblast1
 nomer_elementa_vetvi2++;
 a_ocenki=nomer_elementa_vetvi2;
 ocenka=0;
 massiv_vetveq[vetv2][a_ocenki]=ocenka;
 nomer_elementa_vetvi2++;
 a_kolichestva_object2=nomer_elementa_vetvi2;
 massiv_vetveq[vetv2][a_kolichestva_object2]=0;
 kolichestvo_object1=massiv_vetveq[vetv1][a_kolichestva_object2];
 if(kolichestvo_object1==0) {
   message_exit=110100269;// 110100269 = пустая первичная ветвь
   goto next_vetv_iera_6;
   }
 kolichestvo_object2=0;
 nomer_object1=1;
 nomer_object2=1;
 nomer_elementa_vetvi2++;
 nomer_elementa_vetvi1=nomer_elementa_vetvi2;
 a_nachala=nomer_elementa_vetvi2;
 nomer_zapisi1=1;// запись в сортировочном массиве
 nomer_zapisi2=1;// запись в тройном массиве
next_vetv_iera_1:
 now_object=massiv_vetveq[vetv1][a_nachala+nomer_object1-1];
 if(oblast_object==sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2]) {
   if(now_object==sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]) {
     if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]==0)
       goto next_vetv_iera_1_1_2;
     i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
     i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
     i3=i2+4+i1;
     next_object=massiv_vetveq[vetv1][nomer_elementa_vetvi1+1];
     for(i4=i2+4;i4<i3;i4++) {
       if(next_object==massiv_iera[nomer_iera1][i4]) {
	 goto next_vetv_iera_1_1_1_1;
	 }
       }
     }
   }
next_vetv_iera_1_1_2:
 nomer_zapisi1++;
 if(nomer_zapisi1>chislo_zapiseq_iera) {
   message_exit=110100270;// 110100270 = нет полного соответствия ветви в иерархии
   goto next_vetv_iera_6;
   }
 goto next_vetv_iera_1;
next_vetv_iera_1_1_1_1:
 massiv_zapiseq[nomer_zapisi2][0]=now_object;
 massiv_zapiseq[nomer_zapisi2][1]=nomer_zapisi1;
 massiv_zapiseq[nomer_zapisi2][2]=i4-i2-4;
 massiv_zapiseq[nomer_zapisi2][3]=next_object;
 nomer_zapisi2++;
 nomer_zapisi1=1;
 nomer_object1++;
 nomer_elementa_vetvi1++;
 if(nomer_object1==kolichestvo_object1)
   goto next_vetv_iera_2;
 goto next_vetv_iera_1;
next_vetv_iera_2:
 nomer_zapisi1=1;
 now_object=massiv_zapiseq[nomer_zapisi2-1][3];
next_vetv_iera_2_1:
 if(oblast_object==sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2]) {
   if(now_object==sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]) {
     if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]==0)
       goto next_vetv_iera_2_2_1;
next_vetv_iera_2_1_1:
     i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
     i3=i2+4;
     next_object=massiv_iera[nomer_iera1][i3];
     massiv_zapiseq[nomer_zapisi2][0]=now_object;
     massiv_zapiseq[nomer_zapisi2][1]=nomer_zapisi1;
     massiv_zapiseq[nomer_zapisi2][2]=0;
     massiv_zapiseq[nomer_zapisi2][3]=next_object;
     nomer_zapisi2++;
     nomer_zapisi1=1;
     nomer_object1++;
     now_object=next_object;
     nomer_zapisi1=1;
     goto next_vetv_iera_2_1;
     }
   }
next_vetv_iera_2_2_1:
 nomer_zapisi1++;
 if(nomer_zapisi1>chislo_zapiseq_iera)
   goto next_vetv_iera_3;
 goto next_vetv_iera_2_1;
next_vetv_iera_3:
next_vetv_iera_3_1:
 nomer_zapisi2=nomer_zapisi2-1;
 if(nomer_zapisi2==0) {
   message_exit=110100271;// 110100271 = дерево кончилось, все ветви перебраны
   goto next_vetv_iera_6;
   }
 nomer_zapisi1=massiv_zapiseq[nomer_zapisi2][1];
 i1=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
 i2=massiv_zapiseq[nomer_zapisi2][2];
 i3=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5];
 i2++;
 if(i2>=i3)
   goto next_vetv_iera_3_1;
 massiv_zapiseq[nomer_zapisi2][2]=i2;
 i4=i1+4+i2;
 now_object=massiv_iera[nomer_iera1][i4];
 massiv_zapiseq[nomer_zapisi2][3]=now_object;
 nomer_zapisi2++;
next_vetv_iera_4:
 nomer_zapisi1=1;
next_vetv_iera_4_1:
 if(oblast_object==sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][2]) {
   if(now_object==sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][3]) {
     if(sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][5]==0)
       goto next_vetv_iera_4_2_1;
next_vetv_iera_4_1_1:
     i2=sortirovochnaya_iera[nomer_iera1][nomer_zapisi1][6];
     i3=i2+4;
     next_object=massiv_iera[nomer_iera1][i3];
     massiv_zapiseq[nomer_zapisi2][0]=now_object;
     massiv_zapiseq[nomer_zapisi2][1]=nomer_zapisi1;
     massiv_zapiseq[nomer_zapisi2][2]=0;
     massiv_zapiseq[nomer_zapisi2][3]=next_object;
     nomer_zapisi2++;
     nomer_zapisi1=1;
     nomer_object1++;
     now_object=next_object;
     nomer_zapisi1=1;
     goto next_vetv_iera_4_1;
     }
   }
next_vetv_iera_4_2_1:
 nomer_zapisi1++;
 if(nomer_zapisi1>chislo_zapiseq_iera)
   goto next_vetv_iera_5;
 goto next_vetv_iera_4_1;
next_vetv_iera_5:
 kolichestvo_object2=nomer_zapisi2-1;
 nomer_zapisi2=1;
 massiv_vetveq[vetv2][a_kolichestva_object2]=kolichestvo_object2+1;
 nomer_elementa_vetvi2=8;
next_vetv_iera_5_1:
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=massiv_zapiseq[nomer_zapisi2][0];
 nomer_elementa_vetvi2++;
 i1=massiv_zapiseq[nomer_zapisi2][1];
 ocenka=ocenka+sortirovochnaya_iera[nomer_iera1][i1][4];
 nomer_zapisi2++;
 if(nomer_zapisi2<=kolichestvo_object2)
   goto next_vetv_iera_5_1;
 nomer_zapisi2=nomer_zapisi2-1;
 massiv_vetveq[vetv2][nomer_elementa_vetvi2]=massiv_zapiseq[nomer_zapisi2][3];
 nomer_elementa_vetvi2++;
 massiv_vetveq[vetv2][a_ocenki]=ocenka;
 massiv_vetveq[vetv2][0]=nomer_elementa_vetvi2;
 message_exit=0;
next_vetv_iera_6:
 return(message_exit);
 }

// на выходе функции длина строки в байтах
unsigned int dlina_text_to_return(char *text) {
 int i1,i2;
 i1=0;
dlina_text1:
 i2=text[i1];
 if(i2==0)
   goto dlina_text2;
 i1++;
 goto dlina_text1;
dlina_text2:
 return i1;
 }

// конверсия текста в числовой массив
unsigned int convert_text_to_massiv(char *text, int *massiv) {
 int i1,i2,i3;
 i1=1;
 i2=0;
convert_text_to_massiv1:
 i3=(unsigned char)text[i2];
 if(i3==0)
   goto convert_text_to_massiv2;
 massiv[i1]=i3;
 i2++;
 i1++;
 goto convert_text_to_massiv1;
convert_text_to_massiv2:
 massiv[0]=i1;
 return 110100301;// 110100301 = нормальный выход
}

// конверсия числового массива в текст
unsigned int convert_massiv_to_text(int *massiv, char *text) {
 int i1,i2,i3;
 i1=massiv[0];
 i2=0;
 for(i3=1;i3<i1;i3++) {
   text[i2]=massiv[i3];
   i2++;
   }
 text[i2]=0;
 return 110100302;// 110100302 = нормальный выход
}

// поиск символа в массиве, возвращает место символа
// 0 - некорректная длина массива
// 0 - отсутствие символа в массиве
unsigned int find_simvola_v_massiv_to_return(unsigned int *massiv, unsigned char ch) {
 unsigned int i1,i2,i3;
 i1=massiv[0];
 if(i1<2)
   return 0;
 for(i2=1;i2<i1;i2++) {
   i3=massiv[i2];
   if(i3==ch)
     return i2;
   }
 return 0;
 }

// поиск символа в массиве со стартовой позиции, возвращает место символа
// 0 - некорректная длина массива
// 0 - некорректная стартовая позиция массива
// 0 - отсутствие символа в массиве
unsigned int find_simvola_v_massiv_next_to_return(unsigned int *massiv, unsigned char ch, unsigned int start) {
 unsigned int i1,i2,i3;
 i1=massiv[0];
 if(i1<2)
   return 0;
 if(start>i1)
   return 0;
 for(i2=start;i2<i1;i2++) {
   i3=massiv[i2];
   if(i3==ch)
     return i2;
   }
 return 0;
 }

// поиск int в массиве int, возвращает позицию символа
// 0 - некорректная длина массива int
// 0 - отсутствие int в массиве int
unsigned int find_int_v_massiv_to_return(unsigned int *massiv, unsigned int ch) {
 unsigned int i1,i2,i3;
 i1=massiv[0];
 if(i1<2)
   return 0;
 for(i2=1;i2<i1;i2++) {
   i3=massiv[i2];
   if(i3==ch)
     return i2;
   }
 return 0;
 }

// поиск int в массиве int со стартовой позиции, возвращает позицию символа
// 0 - некорректная длина массива int
// 0 - некорректная стартовая позиция в массиве int
// 0 - отсутствие int в массиве int
unsigned int find_int_v_massiv_next_to_return(unsigned int *massiv, unsigned int ch, unsigned int start) {
 unsigned int i1,i2,i3;
 i1=massiv[0];
 if(i1<2)
   return 0;
 if(start>i1)
   return 0;
 for(i2=start;i2<i1;i2++) {
   i3=massiv[i2];
   if(i3==ch)
     return i2;
   }
 return 0;
 }

// поиск массивов2 в массиве1, результаты в массив3
unsigned int find_massiv_v_massiv_to_massiv(unsigned int *massiv1, unsigned int *massiv2, unsigned int *massiv3) {
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8;
 unsigned int f1;
 i1=massiv1[0];
 if(i1<2)
   return 110100303;
 i2=massiv2[0];
 if(i2<2)
   return 110100304;
 if(i2>i1)
   return 110100305;
 i3=1;//указатель у 1 массива
 i8=1;//указатель у 3 массива
find_massiv_v_massiv_to_massiv1:
 i4=i3;//временный указатель у 1 массива
 i5=1;//указатель у 2 массива
find_massiv_v_massiv_to_massiv2:
 i6=massiv1[i4];
 i7=massiv2[i5];
 f1=0;
 if(i6==i7)
   goto find_massiv_v_massiv_to_massiv3;
 i3++;
 goto find_massiv_v_massiv_to_massiv1;
find_massiv_v_massiv_to_massiv3:
 i5++;
 if(i5>=i2) {
   f1=1;
   massiv3[i8]=i3;
   i8++;
   i3++;
   }
 i4++;//увеличение временного указателя 1 массива
 if(i4>=i1) {
   goto find_massiv_v_massiv_to_massiv4;
   }
 if(f1==1)
   goto find_massiv_v_massiv_to_massiv1;
 goto find_massiv_v_massiv_to_massiv2;
find_massiv_v_massiv_to_massiv4:
 massiv3[0]=i8;
 return 0;
}

// поиск массива2 в массиве1, результат выводится через return
unsigned int find_massiv_v_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2) {
 unsigned int i1,i2,i3,i4,i5,i6,i7;
 unsigned int f1;
 i1=massiv1[0];
 if(i1<2)
   return 0;
 i2=massiv2[0];
 if(i2<2)
   return 0;
 if(i2>i1)
   return 0;
 i3=1;//указатель у 1 массива
find_massiv_v_massiv_to_return1:
 i4=i3;//временный указатель у 1 массива
 i5=1;//указатель у 2 массива
find_massiv_v_massiv_to_return2:
 i6=massiv1[i4];
 i7=massiv2[i5];
 f1=0;
 if(i6==i7)
   goto find_massiv_v_massiv_to_return3;
 i3++;
 goto find_massiv_v_massiv_to_return1;
find_massiv_v_massiv_to_return3:
 i5++;
 if(i5>=i2)
   f1=1;
 i4++;//увеличение временного указателя 1 массива
 if(i4>=i1) {
   goto find_massiv_v_massiv_to_return4;
   }
 if(f1==1)
   return i3;
 goto find_massiv_v_massiv_to_return2;
find_massiv_v_massiv_to_return4:
 return 0;
}

// поиск массива2 в массиве1 со стартовой позиции, результат выводится через return
// 0 - некорректная длина массива1 int
// 0 - некорректная длина массива2 int
// 0 - некорректная длина массива2 int
// 0 - отсутствует массив в массиве
unsigned int find_massiv_v_massiv_next_to_return(unsigned int *massiv1, unsigned int *massiv2, unsigned int start) {
 unsigned int i1,i2,i3,i4,i5,i6,i7;
 unsigned int f1;
 i1=massiv1[0];
 if(i1<2)
   return 0;
 i2=massiv2[0];
 if(i2<2)
   return 0;
 if(i2>i1)
   return 0;
 i3=start;//указатель у 1 массива
find_massiv_v_massiv_to_return1:
 i4=i3;//временный указатель у 1 массива
 i5=1;//указатель у 2 массива
find_massiv_v_massiv_to_return2:
 i6=massiv1[i4];
 i7=massiv2[i5];
 f1=0;
 if(i6==i7)
   goto find_massiv_v_massiv_to_return3;
 i3++;
 goto find_massiv_v_massiv_to_return1;
find_massiv_v_massiv_to_return3:
 i5++;
 if(i5>=i2)
   f1=1;
 i4++;//увеличение временного указателя 1 массива
 if(i4>=i1) {
   goto find_massiv_v_massiv_to_return4;
   }
 if(f1==1)
   return i3;
 goto find_massiv_v_massiv_to_return2;
find_massiv_v_massiv_to_return4:
 return 0;
}

// определение кодировки массива, вовращает значение  
// 27000051=koi8-r
// 27000052=cp1251
// 27000053=ibm866
// 27000054=utf8
// 27000055=utf16
unsigned int opredelenie_kodirovki_to_return(unsigned int *massiv) {
 unsigned int i1,i2,i3;
 unsigned char ch;
 unsigned int flag_koi8r=0,flag_cp1251=0,flag_ibm866=0,flag_utf8=0,flag_utf16=0;
 i1=massiv[0];
 if(i1<2)
   return 110100306;
 for(i2=1;i2<i1;i2++) {
   i3=massiv[i2];
   if(i3==238||i3==232||i3==229||i3==224) { //буквы cp1251 о,и,е,а
     flag_cp1251++;
     goto opredelenie_kodirovki_to_return1;
     }
   if(i3==207||i3==201||i3==197||i3==193) { //буквы koi8-r о,и,е,а
     flag_koi8r++;
     goto opredelenie_kodirovki_to_return1;
     }
   if(i3==209||i3==208) { //буква utf8 я,п
     flag_utf8++;
     goto opredelenie_kodirovki_to_return1;
     }
   if(i3==174||i3==168||i3==165||i3==160) { //буквы ibm866 о,и,е,а
     flag_ibm866++;
     goto opredelenie_kodirovki_to_return1;
     }
   if(i3==4) { //кодовый символ utf16 =04
     flag_utf16++;
     goto opredelenie_kodirovki_to_return1;
     }
opredelenie_kodirovki_to_return1:
   ;
   }
 if(flag_cp1251>=flag_koi8r && flag_cp1251>=flag_ibm866 && flag_cp1251>=flag_utf8 && flag_cp1251>=flag_utf16)
   return 0x28000152;
 if(flag_koi8r>=flag_ibm866 && flag_koi8r>=flag_utf8 && flag_koi8r>=flag_utf16)
   return 0x28000151;
 if(flag_ibm866>=flag_utf8 && flag_ibm866>=flag_utf16)
   return 0x28000153;
 if(flag_utf8>0)
   return 0x28000154;
 if(flag_utf16>0)
   return 0x28000155;
 return 110100307;
 }

// определение наличия набора символов в тексте, второй массив заполняется их порядковым номером в наборе
unsigned int simvoly_v_text(unsigned int *massiv1, unsigned int *massiv_simvol,
  unsigned int *massiv2) {
 int i1,i2,i3,i4,i5,i6;
 i1=massiv_simvol[0];
 i2=massiv1[0];
 for(i3=1;i3<i2;i3++) {
   i4=massiv1[i3];
   for(i5=1;i5<i1;i5++) {
     i6=massiv_simvol[i5];
     if(i4==i6) {
       massiv2[i3]=i5;
       goto simvoly_v_text1;
       }
     massiv2[i3]=0;
     }
simvoly_v_text1:
     ;
   }
 massiv2[0]=i2;
 return 110100308;// 110100308 = нормальный выход
 }

// конверсия текста text1 в text1 по таблицам символов из table1 в table2
unsigned int convert_text1(unsigned int *text1, unsigned int *table1,
  unsigned int *table2, unsigned int *text2) {
 int i1,i2,i3,i4,i5,i6,i7;
 i1=table1[0];
 i2=table2[0];
 i3=text1[0];
 for(i4=1;i4<i3;i4++) {
   i5=text1[i4];
   for(i6=1;i6<i1;i6++) {
     i7=table1[i6];
     if(i5==i7) {
       if(i6>=i2)
         goto convert_text1_1;
       text2[i4]=table2[i6];
       goto convert_text1_2;
       }
     }
convert_text1_1:
   text2[i4]=text1[i4];
convert_text1_2:
   ;
   }
 text2[0]=i3;
 return 110100309;// 110100309 = нормальный выход
 }

// добавление с правой части строки текста right_text_1
unsigned int right_text(char *obschiq_text, char *right_text_1) {
 int i1,i2,i3,i4,i5,i6,i7;
 i1=0;
right_text1:
 i2=right_text_1[i1];
 if(i2==0) goto right_text2;
 i1++;
 goto right_text1;
right_text2:
 i3=0;
right_text3:
 i4=obschiq_text[i3];
 if(i4==0) goto right_text4;
 i3++;
 goto right_text3;
right_text4:
 i5=i1+i3;
 i6=0;
 for(i7=i3;i7<i5;i7++) {
   obschiq_text[i7]=right_text_1[i6];
   i6++;
   }
 obschiq_text[i7]=0;
 return 110100310;// 110100310 = нормальный выход
 }

// добавление с правой части массива right_massiv
unsigned int right_massiv(unsigned int *obschiq_massiv, unsigned int *right_massiv) {
 int i1,i2,i3;
 i1=obschiq_massiv[0];
 if(i1<2)
   return 110100311;
 i2=right_massiv[0];
 if(i2<2)
   return 110100312;
 for(i3=1;i3<i2;i3++) {
   obschiq_massiv[i1]=right_massiv[i3];
   i1++;
   }
 obschiq_massiv[0]=i1;
 return 110100313;// 110100313 = нормальный выход
 }

// добавление с левой части строки текста left_text_1
unsigned int left_text(char *obschiq_text, char *left_text_1) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10;
 i1=0;
left_text1:
 i2=left_text_1[i1];
 if(i2==0) goto left_text2;
 i1++;
 goto left_text1;
left_text2:
 i3=0;
left_text3:
 i4=obschiq_text[i3];
 if(i4==0) goto left_text4;
 i3++;
 goto left_text3;
left_text4:
 i5=i1+i3-1;
 i6=i1-1;
 i7=i3-1;
 i8=i5+1;
 obschiq_text[i8]=0;
 for(i9=i5;i9>i6;i9--) {
   obschiq_text[i9]=obschiq_text[i7];
   i7--;
   }
 for(i10=0;i10<i1;i10++) {
   obschiq_text[i10]=left_text_1[i10];
   }
 return 110100314;// 110100314 = нормальный выход
 }

// добавление с левой части массива left_massiv
unsigned int left_massiv(unsigned int *obschiq_massiv, unsigned int *left_massiv) {
 int i1,i2,i3,i4,i5;
 i1=obschiq_massiv[0];
 if(i1<2)
   return 110100315;
 i2=left_massiv[0];
 if(i2<2)
   return 110100316;
 i3=i1+i2-2;
 for(i4=i3;i4>=i2;i4--) {
   i1--;
   obschiq_massiv[i4]=obschiq_massiv[i1];
   }
 for(i5=1;i5<i2;i5++) {
   obschiq_massiv[i5]=left_massiv[i5];
   }
 obschiq_massiv[0]=i3+1;
 return 110100317;// 110100317 = нормальный выход
 }

// склеивание строки из левой части, средней части и правой части текста
unsigned int mid_text(char *left_text, char *mid_text, char *right_text, char *obschiq_text) {
 int i1,i2,i3,i4,i5,i6,i7;
 i1=0;
 i2=0;
mid_text1:
 i3=left_text[i2];
 if(i3==0) goto mid_text2;
 obschiq_text[i1]=i3;
 i1++;
 i2++;
 goto mid_text1;
mid_text2:
 i4=0;
mid_text3:
 i5=mid_text[i4];
 if(i5==0) goto mid_text4;
 obschiq_text[i1]=i5;
 i1++;
 i4++;
 goto mid_text3;
mid_text4:
 i6=0;
mid_text5:
 i7=right_text[i6];
 if(i7==0) goto mid_text6;
 obschiq_text[i1]=i7;
 i1++;
 i6++;
 goto mid_text5;
mid_text6:
 obschiq_text[i1]=0;
 return 110100318;// 110100318 = нормальный выход
 } 

// склеивание массива из левой части, средней части и правой части
unsigned int mid_massiv(unsigned int *left_massiv, unsigned int *mid_massiv,
  unsigned int *right_massiv, unsigned int *obschiq_massiv) {
 int i1,i2,i3,i4,i5,i6,i7;
 i1=left_massiv[0];
 if(i1<2)
   return 110100319;
 i2=mid_massiv[0];
 if(i2<2)
   return 110100320;
 i3=right_massiv[0];
 if(i3<2)
   return 1101003421;
 for(i4=1;i4<i1;i4++)
   obschiq_massiv[i4]=left_massiv[i4];
 for(i5=1;i5<i2;i5++) {
   obschiq_massiv[i4]=mid_massiv[i5];
   i4++;
   }
 for(i6=1;i6<i3;i6++) {
   obschiq_massiv[i4]=right_massiv[i6];
   i4++;
   }
 obschiq_massiv[0]=i4;
 return 110100322;// 110100322 = нормальный выход
 }

// вырезание справа n количества символов
unsigned int right_rezalka_text(char *obschiq_text, unsigned int n, char *right_rezuemyq_text) {
 int i1,i2,i3,i4;
 i1=0;
right_rezalka_text1:
 i2=right_rezuemyq_text[i1];
 if(i2==0) goto right_rezalka_text2;
 i1++;
 goto right_rezalka_text1;
right_rezalka_text2:
obschiq_text[0]=0;
 if(n>i1)
   return 110100323;
 i3=i1-1;
 for(i4=n-1;i4>=0;i4--) {
   obschiq_text[i4]=right_rezuemyq_text[i3];
   i3--;
   }
 obschiq_text[n]=0;
 return 110100324;// 110100324 = нормальный выход
 }

// вырезание справа n количества элементов массива
unsigned int right_rezalka_massiv(unsigned int *obschiq_massiv, unsigned int n,
  unsigned int *right_rezuemyq_massiv) {
 int i1,i2,i3;
 i1=obschiq_massiv[0];
 if(n>i1)
   return 110100325;
 i2=i1-n;
 for(i3=1;i3<=n;i3++) {
   right_rezuemyq_massiv[i3]=obschiq_massiv[i2];
   i2++;
   }
 right_rezuemyq_massiv[0]=n+1;
 return 110100326;// 110100326 = нормальный выход
 }

// вырезание слева n количества символов
unsigned int left_rezalka_text(char *obschiq_text, unsigned int n, char *left_rezuemyq_text) {
 int i1,i2,i3,i4;
 i1=0;
left_rezalka_text1:
 i2=left_rezuemyq_text[i1];
 if(i2==0) goto left_rezalka_text2;
 i1++;
 goto left_rezalka_text1;
left_rezalka_text2:
obschiq_text[0]=0;
 if(n>i1)
   return 110100327;
 for(i3=0;i3<n;i3++) {
   obschiq_text[i3]=left_rezuemyq_text[i3];
   }
 obschiq_text[n]=0;
 return 110100328;// 110100328 = нормальный выход
 }

// вырезание слева n количества элементов массива
unsigned int left_rezalka_massiv(unsigned int *obschiq_massiv, unsigned int n,
  unsigned int *left_rezuemyq_massiv) {
 int i1,i2,i3;
 i1=obschiq_massiv[0];
 if(n>i1)
   return 110100329;
 for(i1=1;i1<=n;i1++)
   left_rezuemyq_massiv[i1]=obschiq_massiv[i1];
 left_rezuemyq_massiv[0]=n+1;
 return 110100330;// 110100330 = нормальный выход
 }

// сравнение двух строк, копия функции strcmp(x1,x2), но без необходимой для этого библиотеки string.h
// на выходе 1 - если строки совпадают 0 - если нет
unsigned int sravnenie_text_to_return(char *text1, char *text2) {
 int i1,i2,i3,i4,i5,i6,i7;
 int flag_ravenstva;
 i1=0;
 i2=0;
 i3=0;
sravnenie_text_to_return1:
 flag_ravenstva=0;
 i2=text1[i1];
 i3=text2[i1];
 if(i2==i3) {
   flag_ravenstva=1;
   }
   else {
     return 0;
     }
 if(i2==0&&flag_ravenstva==1)
   return 1;
 i1++;
 goto sravnenie_text_to_return1;
 }

// сравнение двух массивов
// на выходе 1 - если массивы совпадают 0 - если нет
unsigned int sravnenie_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2) {
 int i1,i2,i3;
 int flag_ravenstva;
 i1=massiv1[0];
 i2=massiv2[0];
 if(i1!=i2)
   return 0;
 for(i3=1;i3<i1;i3++)
   if(massiv1[i3]!=massiv2[i3])
     return 0;
 return 1;
 }

// нахождение включенности строки в строку
// на выходе 1 - если строка есть в строке 0 - если нет
unsigned int vkluchennost_text_to_return(char *obschiq_text, char *vkluchennyq_text) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10;
 i1=0;
vkluchennost_text_to_return1:
 i2=vkluchennyq_text[i1];
 if(i2==0) goto vkluchennost_text_to_return2;
 i1++;
 goto vkluchennost_text_to_return1;
vkluchennost_text_to_return2:
 if(i1==0)
   return 1;
 i3=0;
vkluchennost_text_to_return3:
 i4=obschiq_text[i3];
 if(i4==0) goto vkluchennost_text_to_return4;
 i3++;
 goto vkluchennost_text_to_return3;
vkluchennost_text_to_return4:
 if(i3<i1)
   return 0;
 i5=0;
 i10=i3-i1;
vkluchennost_text_to_return5:
 i6=0;
 i9=i5;
vkluchennost_text_to_return7:
 i7=vkluchennyq_text[i6];
 i8=obschiq_text[i9];
 if(i7!=i8)
   goto vkluchennost_text_to_return8;
 i6++;
 if(i6==i1)
   return 1;
 i9++;
 if(i9==i3)
   goto vkluchennost_text_to_return8;
 goto vkluchennost_text_to_return7;
vkluchennost_text_to_return8:
 i5++;
 if(i5>i10)
   goto vkluchennost_text_to_return9;
 goto vkluchennost_text_to_return5;
vkluchennost_text_to_return9:
 return 0;
 }

// поиск включенности массива2 в массиве1, результат выводится через return
// 1 - присутствует массив в массиве
// 0 - отсутствует массив в массиве
unsigned int vkluchennost_massiv_v_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2) {
 unsigned int i1,i2,i3,i4,i5,i6,i7;
 unsigned int f1;
 i1=massiv1[0];
 if(i1<2)
   return 110100424;
 i2=massiv2[0];
 if(i2<2)
   return 110100425;
 if(i2>i1)
   return 110100426;
 i3=1;//указатель у 1 массива
vkluchennost_massiv_v_massiv_to_return1:
 i4=i3;//временный указатель у 1 массива
 i5=1;//указатель у 2 массива
vkluchennost_massiv_v_massiv_to_return2:
 i6=massiv1[i4];
 i7=massiv2[i5];
 f1=0;
 if(i6==i7)
   goto vkluchennost_massiv_v_massiv_to_return3;
 i3++;
 goto vkluchennost_massiv_v_massiv_to_return1;
vkluchennost_massiv_v_massiv_to_return3:
 i5++;
 if(i5>=i2)
   f1=1;
 i4++;//увеличение временного указателя 1 массива
 if(i4>=i1) {
   goto vkluchennost_massiv_v_massiv_to_return4;
   }
 if(f1==1)
   return 1;
 goto vkluchennost_massiv_v_massiv_to_return2;
vkluchennost_massiv_v_massiv_to_return4:
 return 0;
}

// нахождение включенности строки справа в строке
// на выходе 1 - если строка есть в строке 0 - если нет
unsigned int vkluchennost_right_text_to_return(char *obschiq_text, char *vkluchennyq_right_text) {
 int i1,i2,i3,i4,i5,i6;
 i1=0;
vkluchennost_right_text_to_return1:
 i2=vkluchennyq_right_text[i1];
 if(i2==0) goto vkluchennost_right_text_to_return2;
 i1++;
 goto vkluchennost_right_text_to_return1;
vkluchennost_right_text_to_return2:
 if(i1==0)
   return 1;
 i3=0;
vkluchennost_right_text_to_return3:
 i4=obschiq_text[i3];
 if(i4==0) goto vkluchennost_right_text_to_return4;
 i3++;
 goto vkluchennost_right_text_to_return3;
vkluchennost_right_text_to_return4:
 if(i3<i1)
   return 0;
 i5=i3-1;
 for(i6=i1-1;i6>=0;i6--) {
   if(vkluchennyq_right_text[i6]!=obschiq_text[i5])
     return 0;
   i5--;
   }
 return 1;
 }

// нахождение включенности массива2 справа в массиве1, результат выводится через return
// 1 - присутствует массив в массиве
// 0 - отсутствует массив в массиве
unsigned int vkluchennost_right_massiv_to_return(unsigned int *obschiq_massiv, unsigned int *vkluchennyq_right_massiv) {
 unsigned int i1,i2,i3,i4;
 unsigned int f1;
 i1=obschiq_massiv[0];
 if(i1<2)
   return 110100332;
 i2=vkluchennyq_right_massiv[0];
 if(i2<2)
   return 110100333;
 if(i2>i1)
   return 110100334;
 i3=i1-1;
 for(i4=i2-1;i4>0;i4--) {
   if(obschiq_massiv[i3]!=vkluchennyq_right_massiv[i4])
     return 0;
   i3--;
   }
 return 1;
 }

// нахождение включенности строки слева в строке
// на выходе 1 - если строка есть в строке 0 - если нет
unsigned int vkluchennost_left_text_to_return(char *obschiq_text, char *vkluchennyq_left_text) {
 int i1,i2,i3,i4,i5,i6;
 i1=0;
vkluchennost_left_text_to_return1:
 i2=vkluchennyq_left_text[i1];
 if(i2==0) goto vkluchennost_left_text_to_return2;
 i1++;
 goto vkluchennost_left_text_to_return1;
vkluchennost_left_text_to_return2:
 if(i1==0)
   return 1;
 i3=0;
vkluchennost_left_text_to_return3:
 i4=obschiq_text[i3];
 if(i4==0) goto vkluchennost_left_text_to_return4;
 i3++;
 goto vkluchennost_left_text_to_return3;
vkluchennost_left_text_to_return4:
 if(i3<i1)
   return 0;
 for(i5=0;i5<i1;i5++) {
   if(vkluchennyq_left_text[i5]!=obschiq_text[i5])
     return 0;
   }
 return 1;
 }

// нахождение включенности массива2 слева в массиве1, результат выводится через return
// 1 - присутствует массив в массиве
// 0 - отсутствует массив в массиве
unsigned int vkluchennost_left_massiv_to_return(unsigned int *obschiq_massiv, unsigned int *vkluchennyq_left_massiv) {
 unsigned int i1,i2,i3;
 unsigned int f1;
 i1=obschiq_massiv[0];
 if(i1<2)
   return 110100335;
 i2=vkluchennyq_left_massiv[0];
 if(i2<2)
   return 110100336;
 if(i2>i1)
   return 110100337;
 for(i3=1;i3<i2;i3++)
   if(obschiq_massiv[i3]!=vkluchennyq_left_massiv[i3])
     return 0;
 return 1;
 }

// разрезание строки на строки по разделительному символу '_'
// возвращает в return количество элементов
// 0 - некорректная длина строки
unsigned int rezalka_text(char *text1, char *text2, char *text3, char *text4,
  char *text5, char *text6, char *text7, char *text8, char *text9, char *text10,
  char *obschiq_text) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13,i14,i15,i16,i17,i18,i19,i20,i21,i22,i23;
 char perevod_slova='_';
 i1=0;
rezalka_text1:
 i2=obschiq_text[i1];
 if(i2==0) goto rezalka_text2;
 i1++;
 goto rezalka_text1;
rezalka_text2:
 if(i1==0)
   return 0;
 i3=0;
// 1 slovo
 text1[0]=0;
 i4=0;
rezalka_text3:
 i5=obschiq_text[i3];
 if(i5==perevod_slova)
   goto rezalka_text4;
 text1[i4]=i5;
 i3++;
 i4++;
 if(i3==i1) {
   text1[i4]=0;
   return 1;
   }
 goto rezalka_text3;
rezalka_text4:
 text1[i4]=0;
 i3++;
// 2 slovo
 text2[0]=0;
 i6=0;
rezalka_text5:
 i7=obschiq_text[i3];
 if(i7==perevod_slova)
   goto rezalka_text6;
 text2[i6]=i7;
 i3++;
 i6++;
 if(i3==i1) {
   text2[i6]=0;
   return 2;
   }
 goto rezalka_text5;
rezalka_text6:
 text2[i6]=0;
 i3++;
// 3 slovo
 text3[0]=0;
 i8=0;
rezalka_text7:
 i9=obschiq_text[i3];
 if(i9==perevod_slova)
   goto rezalka_text8;
 text3[i8]=i9;
 i3++;
 i8++;
 if(i3==i1) {
   text3[i8]=0;
   return 3;
   }
 goto rezalka_text7;
rezalka_text8:
 text3[i8]=0;
 i3++;
// 4 slovo
 text4[0]=0;
 i10=0;
rezalka_text9:
 i11=obschiq_text[i3];
 if(i11==perevod_slova)
   goto rezalka_text10;
 text4[i10]=i11;
 i3++;
 i10++;
 if(i3==i1) {
   text4[i10]=0;
   return 4;
   }
 goto rezalka_text9;
rezalka_text10:
 text4[i10]=0;
 i3++;
// 5 slovo
 text5[0]=0;
 i12=0;
rezalka_text11:
 i13=obschiq_text[i3];
 if(i13==perevod_slova)
   goto rezalka_text12;
 text5[i12]=i13;
 i3++;
 i12++;
 if(i3==i1) {
   text5[i12]=0;
   return 5;
   }
 goto rezalka_text11;
rezalka_text12:
 text5[i12]=0;
 i3++;
// 6 slovo
 text6[0]=0;
 i14=0;
rezalka_text13:
 i15=obschiq_text[i3];
 if(i15==perevod_slova)
   goto rezalka_text14;
 text6[i14]=i15;
 i3++;
 i14++;
 if(i3==i1) {
   text6[i14]=0;
   return 6;
   }
 goto rezalka_text13;
rezalka_text14:
 text6[i14]=0;
 i3++;
// 7 slovo
 text7[0]=0;
 i16=0;
rezalka_text15:
 i17=obschiq_text[i3];
 if(i17==perevod_slova)
   goto rezalka_text16;
 text7[i16]=i17;
 i3++;
 i16++;
 if(i3==i1) {
   text7[i16]=0;
   return 7;
   }
 goto rezalka_text15;
rezalka_text16:
 text7[i16]=0;
 i3++;
// 8 slovo
 text8[0]=0;
 i18=0;
rezalka_text17:
 i19=obschiq_text[i3];
 if(i19==perevod_slova)
   goto rezalka_text18;
 text8[i18]=i19;
 i3++;
 i18++;
 if(i3==i1) {
   text8[i18]=0;
   return 8;
   }
 goto rezalka_text17;
rezalka_text18:
 text8[i18]=0;
 i3++;
// 9 slovo
 text9[0]=0;
 i20=0;
rezalka_text19:
 i21=obschiq_text[i3];
 if(i21==perevod_slova)
   goto rezalka_text20;
 text9[i20]=i21;
 i3++;
 i20++;
 if(i3==i1) {
   text9[i20]=0;
   return 9;
   }
 goto rezalka_text19;
rezalka_text20:
 text9[i20]=0;
 i3++;
// 10 slovo
 text10[0]=0;
 i22=0;
rezalka_text21:
 i23=obschiq_text[i3];
 text10[i22]=i23;
 i3++;
 i22++;
 if(i3==i1) {
   text10[i22]=0;
   return 10;
   }
 goto rezalka_text21;
 }

// разрезание массива на массивы по разделительному символу '_',
// возвращает в return количество элементов
// 0 - некорректная длина массива
unsigned int rezalka_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2, 
  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6, 
  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10, 
  unsigned int *obschiq_massiv) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13;
 i1=obschiq_massiv[0];
 if(i1<2)
   return 0;
 char perevod_slova='_';
 i1--;
 i2=1;
//слово 1
 i3=1;
rezalka_massiv1:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv2;
 massiv1[i3]=i4;
 i3++;
 if(i2>i1) {
   massiv1[0]=i3;
   return 1;
   }
 goto rezalka_massiv1;
rezalka_massiv2:
 massiv1[0]=i3;
//слово 2
 i5=1;
rezalka_massiv3:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv4;
 massiv2[i5]=i4;
 i5++;
 if(i2>i1) {
   massiv2[0]=i5;
   return 2;
   }
 goto rezalka_massiv3;
rezalka_massiv4:
 massiv2[0]=i5;
//слово 3
 i6=1;
rezalka_massiv5:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv6;
 massiv3[i6]=i4;
 i6++;
 if(i2>i1) {
   massiv3[0]=i6;
   return 3;
   }
 goto rezalka_massiv5;
rezalka_massiv6:
 massiv3[0]=i6;
//слово 4
 i7=1;
rezalka_massiv7:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv8;
 massiv4[i7]=i4;
 i7++;
 if(i2>i1) {
   massiv4[0]=i7;
   return 4;
   }
 goto rezalka_massiv7;
rezalka_massiv8:
 massiv4[0]=i7;
//слово 5
 i8=1;
rezalka_massiv9:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv10;
 massiv5[i8]=i4;
 i8++;
 if(i2>i1) {
   massiv5[0]=i8;
   return 5;
   }
 goto rezalka_massiv9;
rezalka_massiv10:
 massiv5[0]=i8;
//слово 6
 i9=1;
rezalka_massiv11:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv12;
 massiv6[i9]=i4;
 i9++;
 if(i2>i1) {
   massiv6[0]=i9;
   return 6;
   }
 goto rezalka_massiv11;
rezalka_massiv12:
 massiv6[0]=i9;
//слово 7
 i10=1;
rezalka_massiv13:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv14;
 massiv7[i10]=i4;
 i10++;
 if(i2>i1) {
   massiv7[0]=i10;
   return 7;
   }
 goto rezalka_massiv13;
rezalka_massiv14:
 massiv7[0]=i10;
//слово 8
 i11=1;
rezalka_massiv15:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv16;
 massiv8[i11]=i4;
 i11++;
 if(i2>i1) {
   massiv8[0]=i11;
   return 8;
   }
 goto rezalka_massiv15;
rezalka_massiv16:
 massiv8[0]=i11;
//слово 9
 i12=1;
rezalka_massiv17:
 i4=obschiq_massiv[i2];
 i2++;
 if(i4==perevod_slova)
   goto rezalka_massiv18;
 massiv9[i12]=i4;
 i12++;
 if(i2>i1) {
   massiv9[0]=i12;
   return 9;
   }
 goto rezalka_massiv17;
rezalka_massiv18:
 massiv9[0]=i12;
//слово 10
 i13=1;
rezalka_massiv19:
 i4=obschiq_massiv[i2];
 i2++;
 massiv10[i13]=i4;
 i13++;
 if(i2>i1) {
   massiv10[0]=i13;
   return 10;
   }
 goto rezalka_massiv19;
 }

// подсчет количества непустых строк, вывод в return
unsigned int kolichestvo_text_to_return(char *text1, char *text2, char *text3, char *text4,
  char *text5, char *text6, char *text7, char *text8, char *text9, char *text10) {
 if(text1[0]==0)
   return 0;
 if(text2[0]==0)
   return 1;
 if(text3[0]==0)
   return 2;
 if(text4[0]==0)
   return 3;
 if(text5[0]==0)
   return 4;
 if(text6[0]==0)
   return 5;
 if(text7[0]==0)
   return 6;
 if(text8[0]==0)
   return 7;
 if(text9[0]==0)
   return 8;
 if(text10[0]==0)
   return 9;
 return 10;
 }

// подсчет количества непустых массивов, вывод в return
unsigned int kolichestvo_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10) {
 if(massiv1[0]<2)
   return 0;
 if(massiv2[0]<2)
   return 1;
 if(massiv3[0]<2)
   return 2;
 if(massiv4[0]<2)
   return 3;
 if(massiv5[0]<2)
   return 4;
 if(massiv6[0]<2)
   return 5;
 if(massiv7[0]<2)
   return 6;
 if(massiv8[0]<2)
   return 7;
 if(massiv9[0]<2)
   return 8;
 if(massiv10[0]<2)
   return 9;
 return 10;
 }

// нахождения совпадения со строкой у группы строк
// на выходе 1 - если строка есть в хотя бы в одной из строк 0 - если нет
unsigned int flag_sovpadeniya_text_to_return(char *text1, char *text2, char *text3, char *text4,
  char *text5, char *text6, char *text7, char *text8, char *text9, char *text10, 
  char *text_sovpadeniya) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 i1=kolichestvo_text_to_return(text1,text2,text3,text4,text5,text6,text7,text8,text9,text10);
 if(i1==0)
   return 0;
 i2=sravnenie_text_to_return(text_sovpadeniya,text1);
 if(i2==1)
   return 1;
 if(i1==1)
   return 0;
 i3=sravnenie_text_to_return(text_sovpadeniya,text2);
 if(i3==1)
   return 1;
 if(i1==2)
   return 0;
 i4=sravnenie_text_to_return(text_sovpadeniya,text3);
 if(i4==1)
   return 1;
 if(i1==3)
   return 0;
 i5=sravnenie_text_to_return(text_sovpadeniya,text4);
 if(i5==1)
   return 1;
 if(i1==4)
   return 0;
 i6=sravnenie_text_to_return(text_sovpadeniya,text5);
 if(i6==1)
   return 1;
 if(i1==5)
   return 0;
 i7=sravnenie_text_to_return(text_sovpadeniya,text6);
 if(i7==1)
   return 1;
 if(i1==6)
   return 0;
 i8=sravnenie_text_to_return(text_sovpadeniya,text7);
 if(i8==1)
   return 1;
 if(i1==7)
   return 0;
 i9=sravnenie_text_to_return(text_sovpadeniya,text8);
 if(i9==1)
   return 1;
 if(i1==8)
   return 0;
 i10=sravnenie_text_to_return(text_sovpadeniya,text9);
 if(i10==1)
   return 1;
 if(i1==9)
   return 0;
 i11=sravnenie_text_to_return(text_sovpadeniya,text10);
 if(i11==1)
   return 1;
 return 0;
 }

// нахождения совпадения с массивом у группы массивов
// на выходе 1 - если массив совпадает хотя бы с одним из массивов 0 - если нет
unsigned int flag_sovpadeniya_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10,
  unsigned int *massiv_sovpadeniya) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 i1=kolichestvo_massiv_to_return(massiv1,massiv2,massiv3,massiv4,massiv5,massiv6,massiv7,massiv8,massiv9,massiv10);
 if(i1==0)
   return 0;
// 1 массив
 i2=sravnenie_massiv_to_return(massiv1,massiv_sovpadeniya);
 if(i2==1)
   return 1;
 if(i1==1)
   return 0;
// 2 массив
 i3=sravnenie_massiv_to_return(massiv2,massiv_sovpadeniya);
 if(i3==1)
   return 1;
 if(i1==2)
   return 0;
// 3 массив
 i4=sravnenie_massiv_to_return(massiv3,massiv_sovpadeniya);
 if(i4==1)
   return 1;
 if(i1==3)
   return 0;
// 4 массив
 i5=sravnenie_massiv_to_return(massiv4,massiv_sovpadeniya);
 if(i5==1)
   return 1;
 if(i1==4)
   return 0;
// 5 массив
 i6=sravnenie_massiv_to_return(massiv5,massiv_sovpadeniya);
 if(i6==1)
   return 1;
 if(i1==5)
   return 0;
// 6 массив
 i7=sravnenie_massiv_to_return(massiv6,massiv_sovpadeniya);
 if(i7==1)
   return 1;
 if(i1==6)
   return 0;
// 7 массив
 i8=sravnenie_massiv_to_return(massiv7,massiv_sovpadeniya);
 if(i8==1)
   return 1;
 if(i1==7)
   return 0;
// 8 массив
 i9=sravnenie_massiv_to_return(massiv8,massiv_sovpadeniya);
 if(i9==1)
   return 1;
 if(i1==8)
   return 0;
// 9 массив
 i10=sravnenie_massiv_to_return(massiv9,massiv_sovpadeniya);
 if(i10==1)
   return 1;
 if(i1==9)
   return 0;
// 10 массив
 i2=sravnenie_massiv_to_return(massiv10,massiv_sovpadeniya);
 if(i2==1)
   return 1;
 return 0;
 }

// нахождения совпадения строки с правой строкой у группы строк
// на выходе 1 - если строка есть в правой строке 0 - если нет
unsigned int flag_sovpadeniya_right_text_to_return(char *text1, char *text2, char *text3,
  char *text4, char *text5, char *text6, char *text7, char *text8, char *text9,
  char *text10, char *text_sovpadeniya) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 i1=kolichestvo_text_to_return(text1,text2,text3,text4,text5,text6,text7,text8,text9,text10);
 if(i1==0)
   return 0;
 if(i1==1) {
   i2=sravnenie_text_to_return(text_sovpadeniya,text1);
   if(i2==1) {
     return 1;
     }
     else {
       return 0;
       }
    }
 if(i1==2) {
   i3=sravnenie_text_to_return(text_sovpadeniya,text2);
   if(i3==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==3) {
   i4=sravnenie_text_to_return(text_sovpadeniya,text3);
   if(i4==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==4) {
   i5=sravnenie_text_to_return(text_sovpadeniya,text4);
   if(i5==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==5) {
   i6=sravnenie_text_to_return(text_sovpadeniya,text5);
   if(i6==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==6) {
   i7=sravnenie_text_to_return(text_sovpadeniya,text6);
   if(i7==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==7) {
   i8=sravnenie_text_to_return(text_sovpadeniya,text7);
   if(i8==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==8) {
   i9=sravnenie_text_to_return(text_sovpadeniya,text8);
   if(i9==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==9) {
   i10=sravnenie_text_to_return(text_sovpadeniya,text9);
   if(i10==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 if(i1==10) {
   i11=sravnenie_text_to_return(text_sovpadeniya,text10);
   if(i11==1) {
     return 1;
     }
     else {
       return 0;
       }
   }
 return 0;
 }

// нахождения совпадения массива с правым массивом у группы массивов
// на выходе 1 - если массив есть в правом массиве 0 - если нет
unsigned int flag_sovpadeniya_right_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10,
  unsigned int *massiv_sovpadeniya) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 i1=kolichestvo_massiv_to_return(massiv1,massiv2,massiv3,massiv4,massiv5,massiv6,massiv7,massiv8,massiv9,massiv10);
 if(i1==0)
   return 0;
// 1 массив
 if(i1==1) {
   i2=sravnenie_massiv_to_return(massiv1,massiv_sovpadeniya);
   if(i2==1)
     return 1;
   return 0;
   }
// 2 массив
 if(i1==2) {
   i3=sravnenie_massiv_to_return(massiv2,massiv_sovpadeniya);
   if(i3==1)
     return 1;
   return 0;
   }
// 3 массив
 if(i1==3) {
   i4=sravnenie_massiv_to_return(massiv3,massiv_sovpadeniya);
   if(i4==1)
     return 1;
   return 0;
   }
// 4 массив
 if(i1==4) {
   i5=sravnenie_massiv_to_return(massiv4,massiv_sovpadeniya);
   if(i5==1)
     return 1;
   return 0;
   }
// 5 массив
 if(i1==5) {
   i6=sravnenie_massiv_to_return(massiv5,massiv_sovpadeniya);
   if(i6==1)
     return 1;
   return 0;
   }
// 6 массив
 if(i1==6) {
   i7=sravnenie_massiv_to_return(massiv6,massiv_sovpadeniya);
   if(i7==1)
     return 1;
   return 0;
   }
// 7 массив
 if(i1==7) {
   i8=sravnenie_massiv_to_return(massiv7,massiv_sovpadeniya);
   if(i8==1)
     return 1;
   return 0;
   }
// 8 массив
 if(i1==8) {
   i9=sravnenie_massiv_to_return(massiv8,massiv_sovpadeniya);
   if(i9==1)
     return 1;
   return 0;
   }
// 9 массив
 if(i1==9) {
   i10=sravnenie_massiv_to_return(massiv9,massiv_sovpadeniya);
   if(i10==1)
     return 1;
   return 0;
   }
// 10 массив
 if(i1==10) {
   i11=sravnenie_massiv_to_return(massiv10,massiv_sovpadeniya);
   if(i11==1)
     return 1;
   return 0;
   }
 return 0;
 }

// нахождения совпадения строки с левой строкой у группы строк
// на выходе 1 - если строка есть в левой строке 0 - если нет
unsigned int flag_sovpadeniya_left_text_to_return(char *text1, char *text2, char *text3,
  char *text4,char *text5, char *text6, char *text7, char *text8, char *text9,
  char *text10, char *text_sovpadeniya) {
 int i1,i2;
 i1=kolichestvo_text_to_return(text1,text2,text3,text4,text5,text6,text7,text8,text9,text10);
 if(i1==0) {
   return 0;
   }
 i2=sravnenie_text_to_return(text_sovpadeniya,text1);
 if(i2==1) {
   return 1;
   }
 return 0;
 }

// нахождения совпадения массива с левым массивом у группы массивов
// на выходе 1 - если массив есть в левом массиве 0 - если нет
unsigned int flag_sovpadeniya_left_massiv_to_return(unsigned int *massiv1, unsigned int *massiv2,
  unsigned int *massiv3, unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9, unsigned int *massiv10,
  unsigned int *massiv_sovpadeniya) {
 int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 i1=kolichestvo_massiv_to_return(massiv1,massiv2,massiv3,massiv4,massiv5,massiv6,massiv7,massiv8,massiv9,massiv10);
 if(i1==0)
   return 0;
// 1 массив
 i2=sravnenie_massiv_to_return(massiv1,massiv_sovpadeniya);
 if(i2==1)
   return 1;
 return 0;
 }

// нахождения совпадения со строкой у группы строк
// на выходе 1 - если строка есть в строках 0 - если нет
// заполняется массив 1 - если строка есть в строке 0 - если нет
unsigned int flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return( char *text1,
  char *text2, char *text3,char *text4, char *text5, char *text6, char *text7,
  char *text8, char *text9, char *text10,char *text_sovpadeniya, int *massiv) {
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 unsigned int f=0;
 i1=kolichestvo_text_to_return(text1,text2,text3,text4,text5,text6,text7,text8,text9,text10);
 massiv[0]=i1+1;
 if(i1==0)
   return 0;
 i2=sravnenie_text_to_return(text_sovpadeniya,text1);
 massiv[1]=i2;
 if(i2==1)
   f=1;
 if(i1==1)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i3=sravnenie_text_to_return(text_sovpadeniya,text2);
 massiv[1]=i3;
 if(i3==1)
   f=1;
 if(i1==2)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i4=sravnenie_text_to_return(text_sovpadeniya,text3);
 massiv[1]=i4;
 if(i4==1)
   f=1;
 if(i1==3)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i5=sravnenie_text_to_return(text_sovpadeniya,text4);
 massiv[1]=i5;
 if(i5==1)
   f=1;
 if(i1==4)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i6=sravnenie_text_to_return(text_sovpadeniya,text5);
 massiv[1]=i6;
 if(i6==1)
   f=1;
 if(i1==5)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i7=sravnenie_text_to_return(text_sovpadeniya,text6);
 massiv[1]=i7;
 if(i7==1)
   f=1;
 if(i1==6)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i8=sravnenie_text_to_return(text_sovpadeniya,text7);
 massiv[1]=i8;
 if(i8==1)
   f=1;
 if(i1==7)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i9=sravnenie_text_to_return(text_sovpadeniya,text8);
 massiv[1]=i9;
 if(i9==1)
   f=1;
 if(i1==8)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i10=sravnenie_text_to_return(text_sovpadeniya,text9);
 massiv[1]=i10;
 if(i10==1)
   f=1;
 if(i1==9)
   goto flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1;
 i11=sravnenie_text_to_return(text_sovpadeniya,text10);
 massiv[1]=i11;
 if(i11==1)
   f=1;
flag_sovpadeniya_text_and_massiv_sovpadeniq_to_return1:
 if(f==1)
   return 1;
 return 0;
 }

// нахождения совпадения массива с группой массивов
// на выходе 1 - если массив есть в каком либо из массивов 0 - если нет
// заполняется массив 1 - если массив совпадает с массивом 0 - если нет
unsigned int flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return(
  unsigned int *massiv1, unsigned int *massiv2, unsigned int *massiv3,
  unsigned int *massiv4, unsigned int *massiv5, unsigned int *massiv6,
  unsigned int *massiv7, unsigned int *massiv8, unsigned int *massiv9,
  unsigned int *massiv10, unsigned int *massiv_sovpadeniya, unsigned int *massiv) {
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11;
 unsigned int f=0;
 i1=kolichestvo_massiv_to_return(massiv1,massiv2,massiv3,massiv4,massiv5,massiv6,massiv7,massiv8,massiv9,massiv10);
 massiv[0]=i1+1;
 if(i1==0)
   return 0;
// 1 массив
 i2=sravnenie_massiv_to_return(massiv1,massiv_sovpadeniya);
 if(i2==1)
   f=1;
 if(i1==1)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 2 массив
 i3=sravnenie_massiv_to_return(massiv2,massiv_sovpadeniya);
 if(i3==1)
   f=1;
 if(i1==2)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 3 массив
 i4=sravnenie_massiv_to_return(massiv3,massiv_sovpadeniya);
 if(i4==1)
   f=1;
 if(i1==3)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 4 массив
 i5=sravnenie_massiv_to_return(massiv4,massiv_sovpadeniya);
 if(i5==1)
   f=1;
 if(i1==4)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 5 массив
 i6=sravnenie_massiv_to_return(massiv5,massiv_sovpadeniya);
 if(i6==1)
   f=1;
 if(i1==5)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 6 массив
 i7=sravnenie_massiv_to_return(massiv6,massiv_sovpadeniya);
 if(i7==1)
   f=1;
 if(i1==6)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 7 массив
 i8=sravnenie_massiv_to_return(massiv7,massiv_sovpadeniya);
 if(i8==1)
   f=1;
 if(i1==7)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 8 массив
 i9=sravnenie_massiv_to_return(massiv8,massiv_sovpadeniya);
 if(i9==1)
   f=1;
 if(i1==8)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 9 массив
 i10=sravnenie_massiv_to_return(massiv9,massiv_sovpadeniya);
 if(i10==1)
   f=1;
 if(i1==9)
   goto flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1;
// 10 массив
 i11=sravnenie_massiv_to_return(massiv10,massiv_sovpadeniya);
 if(i11==1)
   f=1;
flag_sovpadeniya_massiv_and_massiv_sovpadeniq_to_return1:
 if(f==1)
   return 1;
 return 0;
 }

// копирование файла в тест, копирует весь файл включая нули
unsigned int copy_file2text(char *massiv, char *file) {
 FILE *lst1;
 unsigned char ch;
 unsigned int i1;
 if((lst1=fopen(file, "r"))==NULL) {
    return 110100338;
    }
 i1=0;
copy_file2text1:
 fscanf(lst1,"%c",&ch);
 if(feof(lst1))
   goto copy_file2text2;
 massiv[i1]=ch;
 i1++;
 goto copy_file2text1;
copy_file2text2:
 massiv[i1]=0;
 fclose(lst1);
 if(i1==0)
   return 110100339;
 return 110100340;// 110100340 = нормальный выход
 }

// копирование файла в массив int, копирует весь файл включая нули
unsigned int copy_charfile2intmassiv(unsigned int *massiv, char *file) {
 FILE *lst1;
 unsigned char ch;
 unsigned int i1,i2;
 if((lst1=fopen(file, "r"))==NULL) {
    return 110100341;
    }
 i1=1;
copy_charfile2intmassiv1:
 fscanf(lst1,"%c",&ch);
 if(feof(lst1))
   goto copy_charfile2intmassiv2;
 massiv[i1]= (unsigned int) ch;
 i1++;
 goto copy_charfile2intmassiv1;
copy_charfile2intmassiv2:
 massiv[0]=i1;
 fclose(lst1);
 if(i1==1)
   return 110100342;
 return 110100343;// 110100343 = нормальный выход
 }

// копирование строки в файл, нули не копируются
unsigned int copy_text2file(char *massiv, char *file) {
 FILE *lst1;
 char name_file[100000]="./\0";
 unsigned char ch;
 unsigned int i1;
 right_text(name_file,file);
 if((lst1=fopen(name_file, "w"))==NULL) {
    return 110100344;
    }
 i1=0;
copy_text2file1:
 ch=massiv[i1];
 if(ch==0)
   goto copy_text2file2;
 fprintf(lst1,"%c",ch);
 i1++;
 goto copy_text2file1;
copy_text2file2:
 fclose(lst1);
 if(i1==0)
   return 110100345;
 return 110100346;// 110100346 = нормальный выход
 }

// копирование int массива в char файл
unsigned int copy_intmassiv2charfile(unsigned int *massiv, char *file) {
 FILE *lst1;
 char name_file[100000]="./\0";
 unsigned char ch;
 unsigned int i1,i2;
 right_text(name_file,file);
 if((lst1=fopen(name_file, "w"))==NULL) {
    return 110100347;
    }
 i1=massiv[0]-1;
 i2=1;
copy_intmassiv2charfile1:
 ch = (unsigned char) massiv[i2];
 fprintf(lst1,"%c",ch);
 if(i2==i1)
   goto copy_intmassiv2charfile2;
 i2++;
 goto copy_intmassiv2charfile1;
copy_intmassiv2charfile2:
 fclose(lst1);
 if(i1==0)
   return 110100348;
 return 110100349;// 110100349 = нормальный выход
 }

// копирование int массива в int файл
unsigned int copy_intmassiv2intfile(unsigned int *massiv, char *file) {
 FILE *lst1;
 char name_file[100000]="./\0";
 unsigned int i1,i2;
 unsigned int i3,i4;
 unsigned int c1,c2,c3,c4;
 right_text(name_file,file);
 if((lst1=fopen(name_file, "w"))==NULL) {
    return 110100350;
    }
 i1=massiv[0]-1;
 i2=1;
copy_intmassiv2intfile1:
 i4=massiv[i2];
 c1=i4%256;
 i4=i4/256;
 c2=i4%256;
 i4=i4/256;
 c3=i4%256;
 i4=i4/256;
 c4=i4%256;
 fprintf(lst1,"%c%c%c%c",c4,c3,c2,c1);
 if(i2==i1)
   goto copy_intmassiv2intfile2;
 i2++;
 goto copy_intmassiv2intfile1;
copy_intmassiv2intfile2:
 fclose(lst1);
 if(i1==0)
   return 110100351;
 return 110100352;// 110100352 = нормальный выход
 }

// копирование int файла в int массив
unsigned int copy_file2intmassiv(unsigned int *massiv, char *file) {
 FILE *lst1;
 char name_file[100000]="./\0";
 unsigned char ch;
 unsigned int i1;
 unsigned int i2;
 unsigned int c1,c2,c3,c4;
 i1=right_text(name_file,file);
 if((lst1=fopen(name_file, "r"))==NULL) {
    return 110100353;
    }
 i1=1;
copy_file2intmassiv1:
 fscanf(lst1,"%c",&ch);
 if(feof(lst1))
   goto copy_file2intmassiv2;
 c1=(unsigned int) ch;
 i2=c1; 
 fscanf(lst1,"%c",&ch);
 if(feof(lst1))
   goto copy_file2intmassiv2;
 c2=(unsigned int) ch;
 i2=256*i2+c2; 
 fscanf(lst1,"%c",&ch);
 if(feof(lst1))
   goto copy_file2intmassiv2;
 c3=(unsigned int) ch;
 i2=256*i2+c3; 
 fscanf(lst1,"%c",&ch);
 if(feof(lst1))
   goto copy_file2intmassiv2;
 c4=(unsigned int) ch;
 i2=256*i2+c4; 
 massiv[i1]=i2;
 i1++;
goto copy_file2intmassiv1;
copy_file2intmassiv2:
 massiv[0]=i1;
 fclose(lst1);
 if(i1==1)
   return 110100354;
 return 110100355;// 110100355 = нормальный выход
 }

// копирование текста в int массив
unsigned int copy_text2intmassiv(unsigned int *massiv, char *text) {
 unsigned int i1,i2,i3;
 i1=dlina_text_to_return(text);
 i2=1; 
 for(i3=0;i3<i1;i3++) {
   massiv[i2]=text[i3];
   i2++;
   }
 massiv[0]=i3+1;
 if(i1==0)
   return 110100356;
 return 110100357;// 110100357 = нормальный выход
 }

// копирование int массива в текст
unsigned int copy_intmassiv2text(unsigned int *massiv, char *text) {
 unsigned int i1,i2,i3;
 i1=massiv[0]; 
 if(i1==0)
   return 110100358;
 i1--;
 if(i1==0)
   return 110100359;
 i2=1;
 for(i3=0;i3<i1;i3++) {
   text[i3]=massiv[i2];
   i2++;
   }
 massiv[i3]=0;
 return 110100360;// 110100360 = нормальный выход
 }

// копирование двух int массивов в int массив
unsigned int copy_intmassiv1_and_intmassiv2_2intmassiv3(unsigned int *massiv1,
  unsigned int *massiv2, unsigned int *massiv3) {
 unsigned int i1,i2,i3,i4,i5,i6;
 i1=massiv1[0];
 if(i1==0)
   return 110100361;
 i2=massiv2[0];
 if(i2==0)
   return 110100362;
 for(i3=1;i3<i1;i3++) {
   massiv3[i3]=massiv1[i3];
   }
 i4=1;
 i5=i1+i2-1;
 for(i6=i1;i6<i5;i6++) {
   massiv3[i6]=massiv2[i4];
   i4++;
   }
 massiv3[0]=i5;
 if(i5==1)
   return 110100363;
 return 110100364;// 110100364 = нормальный выход
 }

// копирование двух int массивов в первый int массив
unsigned int copy_intmassiv1_and_intmassiv2_2intmassiv1(unsigned int *massiv1,
   unsigned int *massiv2) {
 unsigned int i1,i2,i3,i4,i5;
 i1=massiv1[0];
 if(i1==0)
   return 110100365;
 i2=massiv2[0];
 if(i2==0)
   return 110100366;
 i3=1;
 i4=i1+i2-1;
 for(i5=i1;i5<i4;i5++) {
   massiv1[i5]=massiv2[i3];
   i3++;
   }
 massiv1[0]=i4;
 if(i4==1)
   return 110100367;
 return 110100368;// 110100368 = нормальный выход
 }

// копирование текста1 и текста2 в текст3
unsigned int copy_text1_and_text2_2text3(char *text1, char *text2, char *text3) {
 unsigned int i1,i2,i3,i4,i5;
 i1=dlina_text_to_return(text1);
 i2=dlina_text_to_return(text2);
 for(i3=0;i3<i1;i3++) {
   text3[i3]=text1[i3];
   }
 for(i4=0;i4<i2;i4++) {
   text3[i3]=text2[i4];
   i3++;
   }
 text3[i3]=0;
 if(i3==0)
   return 110100369;
 return 110100370;// 110100370 = нормальный выход
 }

// копирование одного массива int в другой
unsigned int copy_intmassiv1_to_intmassiv2(unsigned int *massiv1, unsigned int *massiv2) {
 unsigned int i1,i2;
 i1=massiv1[0];
 if(i1==0)
   return 110100371;
 for(i2=0;i2<=i1;i2++) {
   massiv2[i2]=massiv1[i2];
   }
 if(i1==1)
   return 110100372;
 return 110100373;// 110100373 = нормальный выход
 }

// копирование одной строки в другую
unsigned int copy_text1_to_text2(char *text1, char *text2) {
 unsigned int i1,i2;
 i1=dlina_text_to_return(text1);
 for(i2=0;i2<=i1;i2++) {
   text2[i2]=text1[i2];
   }
 if(i1==0)
   return 110100374;
 return 110100375;// 110100375 = нормальный выход
 }

// выделение остатка cправа массива int и копирование его в другой массив int
unsigned int ostatok_massiva_int_right(unsigned int *massiv1, unsigned int n,
  unsigned int *massiv2) {
 unsigned int i1,i2,i3;
 i1=massiv1[0];
 if(i1==0)
   return 110100376;
 if(n>i1)
   return 110100377;
 i2=1;
 for(i3=n;i3<i1;i3++) {
   massiv2[i2]=massiv1[i3];
   i2++;
   }
 massiv2[0]=i2;
 if(i3==1)
   return 110100378;
 return 110100379;// 110100379 = нормальный выход
 }

// выделение остатка слева массива int и копирование его в другой массив int
unsigned int ostatok_massiva_int_left(unsigned int *massiv1, unsigned int n,
  unsigned int *massiv2) {
 unsigned int i1,i2,i3,i4;
 i1=massiv1[0];
 if(i1==0)
   return 110100380;
 if(n>i1)
   return 110100381;
 i2=1;
 i3=n;
 for(i4=1;i4<=i3;i4++) {
   massiv2[i2]=massiv1[i4];
   i2++;
   }
 massiv2[0]=i2;
 if(i3==1)
   return 110100382;
 return 110100383;// 110100383 = нормальный выход
 }

// выделение остатка строки и копирование его в другую строку
unsigned int ostatok_text_right(char *text1, unsigned int n, char *text2) {
 unsigned int i1,i2,i3;
 i1=dlina_text_to_return(text1);
 if(n>i1)
   return 110100384;
 i2=0;
 for(i3=n;i3<i1;i3++) {
   text2[i2]=text1[i3];
   i2++;
   }
 text2[i2]=0;
 if(i2==0)
   return 110100385;
 return 110100386;// 110100386 = нормальный выход
 }

// выделение остатка строки и копирование его в другую строку
unsigned int ostatok_text_left(char *text1, unsigned int n, char *text2) {
 int i1,i2,i3,i4,i5;
 i1=dlina_text_to_return(text1);
 if(n>i1)
   return 110100387;
 i2=0;
 i3=i1-n;
 i4=i1-1;
 for(i5=0;i5<i4;i5++) {
   text2[i2]=text1[i5];
   i2++;
   }
 text2[i2]=0;
 if(i2==0)
   return 110100388;
 return 110100389;// 110100389 = нормальный выход
 }

// конверсия массива int по таблицам символов из table1 в table2
unsigned int convert_int1(unsigned int *table1, unsigned int *table2, 
  unsigned int *massiv1, unsigned int *massiv2) {
 int i1,i2,i3,i4,i5,i6,i7;
 i1=table1[0];
 i2=table2[0];
 i3=massiv1[0];
 for(i4=1;i4<=i3;i4++) {
   i5=massiv1[i4];
   for(i6=1;i6<=i1;i6++) {
     i7=table1[i6];
     if(i5==i7) {
       if(i6>=i2)
         goto convert_text1_1;
       massiv2[i4]=table2[i6];
       goto convert_text1_2;
       }
     }
convert_text1_1:
   massiv2[i4]=massiv1[i4];
convert_text1_2:
   ;
   }
 massiv2[0]=massiv1[0];
 return 110100390;// 110100390 = нормальный выход
 }

// конверсия из ibm866 в koi8r из файла в файл
unsigned int convert_ibm866_to_koi8r_file1_to_file2(char *file1, char *file2) {
 unsigned int massiv1[1000];
 unsigned int massiv2[1000];
 unsigned int massiv3[1000000];
 unsigned int massiv4[1000000];
 copy_charfile2intmassiv(massiv1, "./ibm866.txt");
 copy_charfile2intmassiv(massiv2, "./koi8r.txt");
 copy_charfile2intmassiv(massiv3, file1);
 convert_int1(massiv1, massiv2, massiv3, massiv4);
 copy_intmassiv2charfile(massiv4,file2);
 return 110100391;// 110100391 = нормальный выход
 }

// конверсия из cp1251 в koi8r из файла в файл
unsigned int convert_cp1251_to_koi8r_file1_to_file2(char *file1, char *file2) {
 unsigned int massiv1[1000];
 unsigned int massiv2[1000];
 unsigned int massiv3[1000000];
 unsigned int massiv4[1000000];
 copy_charfile2intmassiv(massiv1, "./cp1251.txt");
 copy_charfile2intmassiv(massiv2, "./koi8r.txt");
 copy_charfile2intmassiv(massiv3, file1);
 convert_int1(massiv1, massiv2, massiv3, massiv4);
 copy_intmassiv2charfile(massiv4, file2);
 return 110100392;// 110100392 = нормальный выход
 }

// конверсия из koi8r в cp1251 из файла в файл
unsigned int convert_koi8r_to_cp1251_file1_to_file2(char *file1, char *file2) {
 unsigned int massiv1[1000];
 unsigned int massiv2[1000];
 unsigned int massiv3[1000000];
 unsigned int massiv4[1000000];
 copy_charfile2intmassiv(massiv1, "./koi8r.txt");
 copy_charfile2intmassiv(massiv2, "./cp1251.txt");
 copy_charfile2intmassiv(massiv3, file1);
 convert_int1(massiv1,massiv2,massiv3,massiv4);
 copy_intmassiv2charfile(massiv4, file2);
 return 110100393;// 110100393 = нормальный выход
 }

// конверсия массива int по таблицам символов из utf8 в koi8r
unsigned int convert_int_utf8_to_koi8r(unsigned int *massiv1, unsigned int *massiv2) {
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12,i13,i14;
 unsigned int table1[1000];
 unsigned int table2[1000];
 unsigned int flag=0;
 copy_charfile2intmassiv(table1, "./utf8.txt");
 copy_charfile2intmassiv(table2, "./koi8r.txt");
 i1=table1[0];
 i2=table2[0];
 i3=massiv1[0];
 i4=1;
 i5=1;
 for(i5=1;i5<=i3;i5++) {
   i6=massiv1[i5];
   if(i6==208 || i6==209) {
     for(i7=1;i7<=i1;i7++) {
       i8=table1[i7];
       if(i6==i8) {
         i9=i5+1;
         if(i9>i3) {
           break;
           }
         i10=i7/2+1;
         if(i10>i2) {
           flag=1;
           goto convert_int2_1;
           }
         i11=massiv1[i9];
         i12=i7+1;
	 i13=i12/2+1;
	 if(i13>i2) {
           flag=1;
           goto convert_int2_1;
	   }
	 i14=table1[i12];
	 if(i11==i14) {
	   massiv2[i4]=table2[i10];
           flag=1;
           i4++;
	   }
         }
       i7++;
       }
     flag=1;
     }
convert_int2_1:
   if(flag==1) {
     flag=0;
     i5++;
     }
     else {
       massiv2[i4]=massiv1[i5];
       i4++;
       }
   }
 massiv2[0]=i4-1;
 return 110100394;// 110100394 = нормальный выход
 }

// конверсия из файла в файл по таблицам символов из utf8 в koi8r
unsigned int convert_utf8_to_koi8r_file1_to_file2(char *file1, char *file2) {
 unsigned int massiv1[1000000];
 unsigned int massiv2[1000000];
 copy_charfile2intmassiv(massiv1,file1);
 convert_int_utf8_to_koi8r(massiv1,massiv2);
 copy_intmassiv2charfile(massiv2,file2);
 return 110100395;// 110100395 = нормальный выход
 }

// удаление из массива определенного символа
unsigned int ubiranie_simvola(unsigned int *massiv, unsigned int simvol) {
 unsigned int i1,i2,i3,i4,i5;
 i1=massiv[0];
 if(i1<2)
   return 110100396;
 i2=1;
 i5=1;
 for(i3=1;i3<i1;i3++) {
   i4=massiv[i3];
   if(i4==simvol)
     goto ubiranie_simvola1;
   massiv[i5]=i4;
   i5++;
ubiranie_simvola1:
   ;
   }
 massiv[0]=i5;
 return 110100397;// 110100397 = нормальный выход
 }

// удаление из массива символа 13
unsigned int ubiranie_simvola_13(unsigned int *massiv) {
 unsigned int i1;
 i1=ubiranie_simvola(massiv,13);
 if(i1!=0)
   return 110100398;
 return 110100399;// 110100399 = нормальный выход
 }

// добавление в массиве к 10 символу символа 13
unsigned int dobavlenie_simvola_13(unsigned int *massiv1, unsigned int *massiv2) {
 unsigned int i1,i2,i3,i4;
 i1=massiv1[0];
 if(i1<2)
   return 110100399;
 i2=1;
 for(i3=1;i3<i1;i3++) {
   i4=massiv1[i3];
   if(i4==10) {
     massiv2[i2]=13;
     i2++;
     massiv2[i2]=10;
     goto dobavlenie_simvola_13_1;
     }
   massiv2[i2]=i4;
dobavlenie_simvola_13_1:
   i2++;
   }
 massiv2[0]=i2;
 return 110100400;// 110100400 = нормальный выход
 }

// заполнение упрощенного формата иерархии текстовым массивом
unsigned int zapolnenie_prostoq_iera_text_massivom(unsigned int iera,
  unsigned int *massiv, unsigned int pervichnyq_object) {
 unsigned int i1,i2,i3,i4,i5,i6;
 unsigned int simvol;
 i1=massiv[0];
 if(i1<2)
   return 110100401;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100402;
 i2=iera-0x1000000;
 i3=massiv_iera[i2][1]; 
 if(i3!=zanyataya_iera)
   return 110100403;
 i4=massiv_iera[i2][0];
 i5=i4+4;
 if(i5>=max_dlina_iera)
   return 110100404;
 massiv_iera[i2][i4]=text_banalnoe_format_koi8r;// категория типа текст упрощенный формата koi8-r
 i4++;
 massiv_iera[i2][i4]=pervichnyq_object;
 i4++;
 massiv_iera[i2][i4]=0;
 i4++;
 i5=i4; 
 i4++;
 for(i6=1;i6<i1;i6++) {
   if(i4>=max_dlina_iera)
     return 110100404;
   massiv_iera[i2][i4]=massiv[i6];
   i4++;
   }
 massiv_iera[i2][i5]=i6-1;
 massiv_iera[i2][0]=i4;
 return 110100405;
 }

// заполнение массива int текстом с клавиатуры
unsigned int zapolnenie_massiva_int_textom_keyboard(unsigned int *massiv) {
 unsigned int i1,i2,i3;
 scanf("%s",&massiv_text_keyboard);
 i1=1;
 for(i2=0;i2<1000000;i2++) {
   i3=(unsigned int)massiv_text_keyboard[i2];
   if(i3==0)
     goto zapolnenie_massiva_int_textom_keyboard1;
   massiv[i1]=i3;
   i1++;
   }
 return 110100406;
zapolnenie_massiva_int_textom_keyboard1:
 massiv[0]=i1;
 return 110100407;
 }

// установка флага системы информирующего о том что есть данные в массиве клавиатуры
void system_flag1(int sig) {
 system_rejim1[0]=UES_rejim_keyboard_buffer_full;
 printf("system_rejim1_3=%i\n",system_rejim1[0]);
 }

// вывод на экран int массива как текста
unsigned int vyvod_massiva_int(unsigned int *massiv) {
 unsigned int i1,i2,i3;
 i1=massiv[0];
 for(i2=1;i2<=i1;i2++) {
   i3=massiv[i2];
   printf("%c",i3);
   }
 return 110100408;
 }

// вывод на экран char массива
unsigned int vyvod_massiva_char(char *massiv) {
 printf("%s",massiv);
 return 110100409;
 }

// вывод на экран символа перевода строки
unsigned int vyvod_lf(void) {
 printf("\n");
 return 110100410;
 }

// перевод текстового массива в массив int
unsigned int preobrazovanie_massiva_char_v_massiv_int(char *massiv_char,
  unsigned int *massiv_int) {
 unsigned int i1,i2,i3;
 i1=0;
 i2=1;
preobrazovanie_massiva_char_v_massiv_int1:
 i3=(int)massiv_char[i1];
 if(i3==0)
   goto preobrazovanie_massiva_char_v_massiv_int2;
 massiv_int[i2]=i3;
 i1++;
 i2++;
 goto preobrazovanie_massiva_char_v_massiv_int1;
preobrazovanie_massiva_char_v_massiv_int2:
 massiv_int[0]=i2;
 return 110100411;
 }
 
// перевод массива int в текстовый массив
unsigned int preobrazovanie_massiva_int_v_massiv_char(unsigned int *massiv_int,
  char *massiv_char) {
 unsigned int i1,i2,i3,i4;
 i1=massiv_int[0];
 if(i1==0)
   return 110100412;// ошибка массива
 i2=1;
 i3=0;
preobrazovanie_massiva_int_v_massiv_char1:
 i4=massiv_int[i2];
 massiv_char[i3]=(char)i4;
 if(i4==0)
   goto preobrazovanie_massiva_int_v_massiv_char2;
 i2++;
 i3++;
 if(i2==i1) {
   goto preobrazovanie_massiva_int_v_massiv_char2;
   }
 goto preobrazovanie_massiva_int_v_massiv_char1;
preobrazovanie_massiva_int_v_massiv_char2:
 massiv_char[i3]=(char)0;
 return 110100413;
 }

// помещение в текстовый массив справочной информации монитора
unsigned int monitor_help_info(char *massiv_char2) {
 unsigned int i1,i2,i3;
 char massiv_char1[1000];
 char h01[]="c - пустая команда\n\0";
 char h02[]="h - вывод справочной информации общее\n\0";
 char h03[]="r - запуск выполнения задания в иерархии первичного алгоритма Shell для УЭС\n\0";
 char h04[]="s - остановка выполнения задания в иерархии первичного алгоритма Shell для УЭС\n\0";
 char h05[]="q - выход из программы УЭС без сохранения результатов в иерархиях\n\0";
 char h06[]="w - выход из программы УЭС с сохранением результатов в иерархиях, ветвях и массивах\n\0";
 char h07[]="l - загрузка\n\0";
 char h08[]="lс - загрузка команды\n\0";
 char h09[]="lf - загрузка файла в массив\n\0";
 char h10[]="ls - загрузка результатов в иерархиях, ветвях и массивах\n\0";
 char h11[]="t - обучение информации общее или трансляция\n\0";
 char h12[]="ttk - обучение текстовым фактам с клавиатуры\n\0";
 char h13[]="ttf<file> - обучение текстовым фактам из файла\n\0";
 char h14[]="tikd - обучение int фактам с клавитауры десятичное\n\0";
 char h15[]="tikh - обучение int фактам с клавитауры шестнадцатиричное\n\0";
 char h16[]="tif<file> - обучение int фактам из файла\n\0";
 char h17[]="tii<nomer> - обучение int фактам из иерархии\n\0";
 char h18[]="trh - трансляция шестнадцатиричной цифры в десятичную\n\0";
 char h19[]="trd - трансляция десятичной цифры в шестнадцатиричную\n\0";
 char h20[]="v - вывод рабочей информации общее\n\0";
 char h21[]="vi  - вывод текущей системной переменной\n\0";
 char h22[]="vmi  - вывод текущего системного массива int\n\0";
 char h23[]="vmt  - вывод текущего системного массива text\n\0";
 char h24[]="va  - вывод первичного алгоритма\n\0";
 char h25[]="va<nomer>  - вывод алгоритма <nomer>\n\0";
 char h26[]="vсi  - вывод каталога иерархий\n\0";
 char h27[]="vсv  - вывод каталога ветвей\n\0";
 char h28[]="vi<nomer>  - вывод иерархии <nomer>\n\0";
 char h29[]="vo<nomer> - вывод объекта <nomer>\n\0";
 char h30[]="vni<name> - вывод иерархии с именем и её имя\n\0";
 char h31[]="vno<name> - вывод объекта с именем и его имя\n\0";
 char h32[]="i - загрузка рабочей информации общее\n\0";
 char h33[]="imkd - загрузка текущего int массива с клавиатуры десятичное\n\0";
 char h34[]="imkh - загрузка текущего int массива с клавиатуры шестнадцатиричное\n\0";
 char h35[]="imf<file> - загрузка текущего int массива из файла\n\0";
 char h36[]="imi<nomer> - загрузка текущего int массива из иерархии <nomer>\n\0";
 char h37[]="imv<nomer> - загрузка текущего int массива из ветви <nomer>\n\0";
 char h38[]="iikd - загрузка текущего int массива с клавиатуры десятичное\n\0";
 char h39[]="iikh - загрузка текущего int массива с клавиатуры шестнадцатиричное\n\0";
 char h40[]="iif<file> - загрузка текущего int массива из файла\n\0";
 char h41[]="iii<nomer> - загрузка текущего int массива из иерархии <nomer>\n\0";
 char h42[]="o - выгрузка рабочей информации общее\n\0";
 char h43[]="omf<file> - выгрузка текущего int массива в файл\n\0";
 char h44[]="omi<nomer> - выгрузка текущего int массива в иерархию <nomer>\n\0";
 char h45[]="omv<nomer> - выгрузка текущего int массива в ветвь <nomer>\n\0";
 char h46[]="otf<file> - выгрузка текущего text массива в файл\n\0";
 char h47[]="oti1 - выгрузка текущего text массива в иерархию\n\0";
 copy_text1_to_text2(h01, massiv_char1);
 copy_text1_to_text2(massiv_char1, massiv_char2);
 right_text(massiv_char2, h02);
 right_text(massiv_char2, h03);
 right_text(massiv_char2, h04);
 right_text(massiv_char2, h05);
 right_text(massiv_char2, h06);
 right_text(massiv_char2, h07);
 right_text(massiv_char2, h08);
 right_text(massiv_char2, h09);
 right_text(massiv_char2, h10);
 right_text(massiv_char2, h11);
 right_text(massiv_char2, h12);
 right_text(massiv_char2, h13);
 right_text(massiv_char2, h14);
 right_text(massiv_char2, h15);
 right_text(massiv_char2, h16);
 right_text(massiv_char2, h17);
 right_text(massiv_char2, h18);
 right_text(massiv_char2, h19);
 right_text(massiv_char2, h20);
 right_text(massiv_char2, h21);
 right_text(massiv_char2, h22);
 right_text(massiv_char2, h23);
 right_text(massiv_char2, h24);
 right_text(massiv_char2, h25);
 right_text(massiv_char2, h26);
 right_text(massiv_char2, h27);
 right_text(massiv_char2, h28);
 right_text(massiv_char2, h29);
 right_text(massiv_char2, h30);
 right_text(massiv_char2, h31);
 right_text(massiv_char2, h32);
 right_text(massiv_char2, h33);
 right_text(massiv_char2, h34);
 right_text(massiv_char2, h35);
 right_text(massiv_char2, h36);
 right_text(massiv_char2, h37);
 right_text(massiv_char2, h38);
 right_text(massiv_char2, h39);
 right_text(massiv_char2, h40);
 right_text(massiv_char2, h41);
 right_text(massiv_char2, h42);
 right_text(massiv_char2, h43);
 right_text(massiv_char2, h44);
 right_text(massiv_char2, h45);
 right_text(massiv_char2, h46);
 right_text(massiv_char2, h47);
 return 110100414;
 }

// перевод с определенной позиции int массива в текстовую строку
unsigned int perevod_int_massiva_pozition_v_text(unsigned int *massiv, unsigned int n,
  char *text) {
unsigned int i1,i2,i3;
 i1=massiv[0];
 if(i1==0||i1==1)
   return 110100415;// некорректный массив
 if(n+1>i1)
   return 110100416;// некорректная стартовая позиция
 i2=0;
 for(i3=n;i3<i1;i3++) {
   text[i2]=massiv[i3];
   i2++;
   }
 text[i2]=0;
 return 110100417;
 }

// перевод текстовой строки в строку с путем к файлу и в строку с именем файла
unsigned int perevod_text_v_namepath_i_namefile(char *text, char *namepath,
  char *namefile) {
 unsigned int i1,i2,i3,i4=0;
 char a;
 unsigned int flag_tekuschiq_dir=0;
 unsigned int flagroot=0;
 unsigned int flag_file=0;
 unsigned int flag_dir=0;
 unsigned int flagerror=0;
 i1=dlina_text_to_return(text);
 if(i1==0)
   return 110100418;//ошибочная строка
 i2=0;
 a=text[i2];
 if(a==simvol_tochki) {
   flag_tekuschiq_dir=1;
   goto perevod_text_v_namepath_i_namefile1;
   }
 if(a==simvol_cataloga) {
   flagroot=1;
   goto perevod_text_v_namepath_i_namefile1;
   }
 flag_file=1;   
perevod_text_v_namepath_i_namefile1:
 for(i3=1;i3<i1;i3++) {
   a=text[i3];
   if(a==simvol_cataloga) {
     i4=i3;
     flag_dir=1;
     }
   }
 if(i4==i1-1) 
   return 110100419;//ошибочная строка
 if(flag_tekuschiq_dir==0&&flagroot==0&&flag_dir==0) {
   namepath[0]='.';
   namepath[1]=(char)0;
   goto perevod_text_v_namepath_i_namefile2;
   }
 if(flagroot==1&&flag_dir==0) {
   namepath[0]=simvol_cataloga;
   namepath[1]=(char)0;
   goto perevod_text_v_namepath_i_namefile2;
   }
 if(flagroot==1&&flag_dir==1) {
   for(i3=0;i3<i4;i3++)
     namepath[i3]=text[i3];
   namepath[i3]=(char)0;
   goto perevod_text_v_namepath_i_namefile2;
   }
 if(flag_tekuschiq_dir==1&&flag_dir==1) {
   for(i3=0;i3<i4;i3++)
     namepath[i3]=text[i3];
   namepath[i3]=(char)0;
   goto perevod_text_v_namepath_i_namefile2;
   }
 return 110100420;//ошибочная строка
perevod_text_v_namepath_i_namefile2://имя файла в массив
 if(flag_file==0)
   i4++;
 if(i4>=i1)
   return 110100421;//ошибочная строка
 for(i3=i4;i3<i1;i3++) {
   namefile[i2]=text[i3];
   i2++;
   }
 namefile[i2]=(char)0;
 return 110100422;
 }

// перевод десятичной текстовой цифры в массиве int в int цифру возращаемую в return
unsigned int perevod_text_10cifry_v_int_to_return(unsigned int *massiv) {
 unsigned int i1,i2,i3,i4,i5=0;
 i1=massiv[0];
 i2=1;
perevod_text_10cifry_v_int_to_return2:
 i3=massiv[i2];
 if(i3<48||i3>57)
   goto perevod_text_10cifry_v_int_to_return1;
 i4=i3-48;
 i5=i5+i4;
 i2++;
 if(i2>=i1)
   goto perevod_text_10cifry_v_int_to_return1;
 i5=i5*10;
   goto perevod_text_10cifry_v_int_to_return2;
perevod_text_10cifry_v_int_to_return1:
 return i5;
 }

// перевод шестнадцатиричной текстовой цифры в массиве int в int цифру возращаемую в return
unsigned int perevod_text_16cifry_v_int_to_return(unsigned int *massiv) {
 unsigned int i1,i2,i3,i4,i5=0;
 i1=massiv[0];
 i2=1;
perevod_text_16cifry_v_int_to_return2:
 i3=massiv[i2];
 if(i3>=48&&i3<=57) {
   i4=i3-48;
   goto perevod_text_16cifry_v_int_to_return3;
   }
 if(i3>=65&&i3<=70) {
   i4=i3-55;
   goto perevod_text_16cifry_v_int_to_return3;
   }
 if(i3>=97&&i3<=102) {
   i4=i3-87;
   goto perevod_text_16cifry_v_int_to_return3;
   }
 goto perevod_text_16cifry_v_int_to_return1;
perevod_text_16cifry_v_int_to_return3:
 i5=i5+i4;
 i2++;
 if(i2>=i1)
   goto perevod_text_16cifry_v_int_to_return1;
 i5=i5*16;
   goto perevod_text_16cifry_v_int_to_return2;
perevod_text_16cifry_v_int_to_return1:
 return i5;
 }

// перевод int цифры в int массив шестнадцатиричной текстовой цифры
unsigned int perevod_int_cifry_v_int_16rich_massiv(unsigned int *massiv, 
  unsigned int cifra) {
 unsigned int i1,i2,i3,i4,i5=0;
 i1=1;
 i2=cifra;
perevod_int_cifry_v_int_16rich_massiv1:
 i2=i2/16;
 if(i2>0) {
   i1++;
   goto perevod_int_cifry_v_int_16rich_massiv1;
   }
 i4=cifra;
 for(i3=i1;i3>0;i3--) {
   i5=i4%16;
   if(i5<10)
     i5=i5+48;
     else
       i5=i5+55;
   massiv[i3]=i5;
   i4=i4/16;
   }
 massiv[0]=i1+1;
 return 110100423;
 }

// печать int массива по символам
unsigned int print_int_text_massiv(unsigned int *massiv) {
 unsigned int i1,i2;
 i1=massiv[0];
 for(i2=1;i2<i1;i2++)
   printf("%c",massiv[i2]);
 return 110100446;
 }

// печать шестнадцатиричного значения int
unsigned int print_16_int(unsigned int cifra) {
 unsigned int massiv[10];
 perevod_int_cifry_v_int_16rich_massiv(massiv,cifra);
 print_int_text_massiv(massiv);
 return 110100447;
 }

// преобразование простого текста в форматированный в массиве
unsigned int copy_unformat_text_to_UES_text(unsigned int *massiv2,
 unsigned int *massiv1, unsigned int text_codepage) {
 unsigned int i1,i2,i3,i4;
 i1=massiv1[0];
 if(i1<2)
   return 110100448;// 110100448 = пустой массив
 i2=1;
 if(text_codepage==0x28000151) // текст формата koi8-r
   goto copy_unformat_text_to_UES_text1;
 if(text_codepage==0x28000152) // текст формата cp1251
   goto copy_unformat_text_to_UES_text2;
 if(text_codepage==0x28000153) // текст формата ibm866
   goto copy_unformat_text_to_UES_text3;
 if(text_codepage==0x28000154) // текст формата utf8
   goto copy_unformat_text_to_UES_text4;
 if(text_codepage==0x28000155) // текст формата utf16
   goto copy_unformat_text_to_UES_text5;
 return 110100449;// 110100449 = некорректная кодовая страница    
copy_unformat_text_to_UES_text1:
 for(i2=1;i2<=i1;i2++)
   massiv2[i2]=massiv1[i2]+0x30303000;// таблица koi8-r
 massiv2[0]=i1;
 goto copy_unformat_text_to_UES_text6;
copy_unformat_text_to_UES_text2:
 for(i2=1;i2<=i1;i2++)
   massiv2[i2]=massiv1[i2]+0x30303100;// таблица cp1251
 massiv2[0]=i1;
 goto copy_unformat_text_to_UES_text6;
copy_unformat_text_to_UES_text3:
 for(i2=1;i2<=i1;i2++)
   massiv2[i2]=massiv1[i2]+0x30303200;// таблица ibm866
 massiv2[0]=i1;
 goto copy_unformat_text_to_UES_text6;
copy_unformat_text_to_UES_text4:
 i3=1;
 for(i2=1;i2<=i1;i2++) {
   i4=massiv1[i2];
   i2++;
   massiv1[i3]=i4+256*massiv1[i2]+0x30310000;// таблица utf8
   i3++;
   }
 massiv2[0]=i3;
copy_unformat_text_to_UES_text5:
 i3=3;
 for(i2=1;i2<=i1;i2++) {
   i4=massiv1[i2];
   i2++;
   massiv1[i3]=i4+256*massiv1[i2]+0x30320000;// таблица utf16
   i3++;
   }
 massiv2[0]=i3;
copy_unformat_text_to_UES_text6:
 return 110100450;
 }

// преобразование простого текста в форматированный в массиве
unsigned int copy_UES_text_to_unformat_text(unsigned int *massiv2,
  unsigned int *massiv1) {
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8;
 i1=massiv1[0];
 if(i1<2)
   return 110100451;// 110100451 = пустой массив
 i2=1;
 i8=massiv1[1];
 if(i8>=0x30320000 && i8<0x30330000) {
   massiv2[i2]=0x0ff;
   i2++;
   massiv2[i2]=0x0fe;
   i2++;
   }
 for(i3=1;i3<i1;i3++) {
   i4=massiv1[i3];
   if(i4>=0x30303000 && i4<0x30303100) {// текст формата koi8-r
     massiv2[i2]=i4-0x30303000;
     i2++;
     goto copy_UES_text_to_unformat_text1;
     }
   if(i4>=0x30303100 && i4<0x30303200) {// текст формата cp1251
     massiv2[i2]=i4-0x30303100;
     i2++;
     goto copy_UES_text_to_unformat_text1;
     }
   if(i4>=0x30303200 && i4<0x30303300) {// текст формата ibm866
     massiv2[i2]=i4-0x30303200;
     i2++;
     goto copy_UES_text_to_unformat_text1;
     }
   if(i4>=0x30310000 && i4<0x30320000) {// текст формата utf8
     i5=i4-0x30310000;
     i6=i5/256;
     i7=i5-256*i6;
     massiv2[i2]=i7;
     i2++;
     massiv2[i2]=i6;
     i2++;
     goto copy_UES_text_to_unformat_text1;
     }
   if(i4>=0x30320000 && i4<0x30330000) {// текст формата utf16
     i5=i4-0x30320000;
     i6=i5/256;
     i7=i5-256*i6;
     massiv2[i2]=i7;
     i2++;
     massiv2[i2]=i6;
     i2++;
     }
   massiv2[0]=i2;
   return 110100452;// нестандартный символ
copy_UES_text_to_unformat_text1:
   ;
   }
 massiv2[0]=i2;
 return 110100453;
 }

//=====
// копирование текстового массива в иерархию 
unsigned int copy_from_UES_text_to_iera(unsigned int iera,
  unsigned int pervichnyq_object, unsigned int *massiv) {
 unsigned int i1,i2,i3,i4,i5;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100454;// 110100454 = нет данной иерархии
 i3=massiv[0];
 if(i3<2)
   return 110100455;// 110100455 = пустой массив
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 i2=i1;
 massiv_iera[iera][i2]=text_banalnoe_format_koi8r;// блок массива текст_банальное_формат_koi8-r
 i2++;
 massiv_iera[iera][i2]=pervichnyq_object;
 i2++;
 massiv_iera[iera][i2]=1;
 i2++;
 massiv_iera[iera][i2]=i3-1;
 i2++;
 for(i4=1;i4<i3;i4++) {
   massiv_iera[iera][i2]=massiv[i4];
   i2++;
   }
 i5=massiv_iera[iera][2];
 i5++;
 massiv_iera[iera][2]=i5;
 return 110100456;
 }

// копирование из иерархии в текстовый массив
unsigned int copy_from_iera_to_UES_text(unsigned int *massiv, unsigned int iera,
  unsigned int pervichnyq_object) {
 unsigned int i1,i2,i3,i4,i5;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100457;// 110100457 = нет данной иерархии
 i1=poisk_kategorii_i_opornogo_object(iera, text_banalnoe_format_koi8r, pervichnyq_object);
 if(i1==0)
   return 110100458;// 110100458 = нет записи
 iera=iera-0x1000000;
 i2=i1+3;
 i3=massiv_iera[iera][i2];
 if(i3==0) {
   massiv[0]=1;
   return 110100459;// 110100459 = нет вторичных объектов
   }
 i2++;
 for(i4=1;i4<=i3;i4++) {
   massiv[i4]=massiv_iera[iera][i2];
   i2++;
   }
 massiv[0]=i3+1;
 return 110100460;
 }

//=====
// запись состояния УЭС
unsigned int save_sostoyanie_UES(void) {
 unsigned int i1,i2=0,i3,i4;
 unsigned int flag_zagolovka;
 int lst1;
 unsigned int max_nomer_rejima;
 unsigned int massiv_rejima[5]={5,0x28000161,0x28000162,0x28000163,0x28000160};
 unsigned int nomer_rejima;
 unsigned int flag_rejima;
 unsigned int nomer_massiva;
 unsigned int nomer_v_massive;
 unsigned int nomer_v_buffer=0;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_path[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_spec_path[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_savefile1[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 obschee_name_file[i2]=0;
 lst1=open(obschee_name_file, O_WRONLY|O_CREAT);
 if(lst1<0) {
   return 110100430;// 110100430 = ошибка открытия файла
   }
 max_nomer_rejima=massiv_rejima[0];
 nomer_rejima=1;
save_sostoyanie_UES1://загрузка нового режима
 flag_rejima=massiv_rejima[nomer_rejima];
 nomer_massiva=1;
save_sostoyanie_UES50://загрузка нового массива
 nomer_v_massive=0;
 flag_zagolovka=1;
save_sostoyanie_UES2:
 if(flag_rejima==0x28000161) {//режим записи иерархий
   if(massiv_iera[nomer_massiva][1]==svobodnaya_iera) {
     nomer_massiva++;
     if(nomer_massiva>max_chislo_iera) {
       nomer_rejima++;
       goto save_sostoyanie_UES1;
       }
     goto save_sostoyanie_UES2;
     }
   i4=massiv_iera[nomer_massiva][0];
   goto save_sostoyanie_UES10;
   }
save_sostoyanie_UES3:
 if(flag_rejima==0x28000162) {//режим записи ветвей
   if(massiv_vetveq[nomer_massiva][1]==svobodnaya_vetv) {
     nomer_massiva++;
     if(nomer_massiva>max_chislo_vetveq) {
       nomer_rejima++;
       goto save_sostoyanie_UES1;
       }
     goto save_sostoyanie_UES3;
     }
   i4=massiv_vetveq[nomer_massiva][0];
   goto save_sostoyanie_UES10;
   }
save_sostoyanie_UES4:
 if(flag_rejima==0x28000163) {//режим записи массивов
   if(BZ100000_be_RAM[nomer_massiva]==0) {
     nomer_massiva++;
     if(nomer_massiva>max_kolichestvo_BZ100000) {
       nomer_rejima++;
       goto save_sostoyanie_UES1;
       }
     goto save_sostoyanie_UES4;
     }
   i4=BZ100000[nomer_massiva][0];
   goto save_sostoyanie_UES10;
   }
save_sostoyanie_UES5:
 if(flag_rejima==0x28000160) {//режим выхода с записью остатка
   if(nomer_v_buffer>0)
     goto save_sostoyanie_UES30;
   goto save_sostoyanie_UES40;
   }
save_sostoyanie_UES10:
 if(flag_zagolovka==1) {
   if(flag_rejima==0x28000161) {//режим записи иерархий
     buffer[nomer_v_buffer]=0x1000000+nomer_massiva;
     goto save_sostoyanie_UES11;
     }
   if(flag_rejima==0x28000162) {//режим записи ветвей
     buffer[nomer_v_buffer]=0x3000000+nomer_massiva;
     goto save_sostoyanie_UES11;
     }
   if(flag_rejima==0x28000163) {//режим записи массивов
     buffer[nomer_v_buffer]=0x75000000+nomer_massiva;
     }
save_sostoyanie_UES11:
   flag_zagolovka=0;
   nomer_v_buffer++;
   if(nomer_v_buffer>=BUFFER_SIZE)
     goto save_sostoyanie_UES20;
   }
save_sostoyanie_UES12:
 if(flag_rejima==0x28000161) {//режим записи иерархий
   buffer[nomer_v_buffer]=massiv_iera[nomer_massiva][nomer_v_massive];
   goto save_sostoyanie_UES13;
   }
 if(flag_rejima==0x28000162) {//режим записи ветвей
   buffer[nomer_v_buffer]=massiv_vetveq[nomer_massiva][nomer_v_massive];
   goto save_sostoyanie_UES13;
   }
 if(flag_rejima==0x28000163) {//режим записи массивов
   buffer[nomer_v_buffer]=BZ100000[nomer_massiva][nomer_v_massive];
   }
save_sostoyanie_UES13:
 nomer_v_massive++;
 nomer_v_buffer++;
 if(nomer_v_buffer>=BUFFER_SIZE)
   goto save_sostoyanie_UES20;
save_sostoyanie_UES14:
 if(nomer_v_massive<i4)
   goto save_sostoyanie_UES12;
 nomer_massiva++;
 if(flag_rejima==0x28000161) {//режим записи иерархий
   if(nomer_massiva>=max_chislo_iera) {
     nomer_rejima++;
     goto save_sostoyanie_UES1;
     }
   }
 if(flag_rejima==0x28000162) {//режим записи ветвей
   if(nomer_massiva>=max_chislo_vetveq) {
     nomer_rejima++;
     goto save_sostoyanie_UES1;
     }
   }
 if(flag_rejima==0x28000163) {//режим записи массивов
   if(nomer_massiva>=max_kolichestvo_BZ100000) {
     nomer_rejima++;
     goto save_sostoyanie_UES1;
     }
   }
 goto save_sostoyanie_UES50;
save_sostoyanie_UES20://сохранение целого буфера
 write(lst1,buffer,INT_BUFFER_SIZE);
 nomer_v_buffer=0;
 goto save_sostoyanie_UES14;
save_sostoyanie_UES30://сохранение остатка буфера
 write(lst1,buffer,nomer_v_buffer*4);
save_sostoyanie_UES40://выход
 unsigned int temp_massiv[1]={0x28000160};//метка конца файла состояния УЭС
 write(lst1,temp_massiv,4);
 close(lst1);
 return 110100431;// 110100431 = нормальный выход
 }

// загрузка состояния УЭС
unsigned int load_sostoyanie_UES(void) {
 unsigned int i1,i2=0,i3,i4,i5,i6;
 int lst1;
 ssize_t read_bytes;
 unsigned int flag_zagruzki_zagolovka=1;
 unsigned int flag_zagruzki_dliny_massiva=0;
 unsigned int flag_zagruzki_iera=0;
 unsigned int flag_zagruzki_vetveq=0;
 unsigned int flag_zagruzki_massivov=0;
 unsigned int nomer_massiva=0;
 unsigned int dlina_massiva=0;
 unsigned int nomer_v_massive=0;
 unsigned int nomer_v_buffer;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_path[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_spec_path[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_savefile1[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 obschee_name_file[i2]=0;
 lst1=open(obschee_name_file, O_RDONLY);
 if(lst1<0) {
   return 110100432;// 110100432 = ошибка открытия файла
   }
 while ((read_bytes=read (lst1,buffer,INT_BUFFER_SIZE))>0) {
   if (read_bytes < 0) {
     return 110100433;// 110100433 = ошибка чтения файла
     }
   i4=read_bytes%4;
   if(i4!=0)
     return 110100434;// 110100434 = ошибка в файле
   i5=read_bytes/4;
   nomer_v_buffer=0;
load_sostoyanie_UES1:
     i6=buffer[nomer_v_buffer];
   if(flag_zagruzki_zagolovka==1) {
     if(i6==0x28000160) {// метка конца файла состояния УЭС
       goto load_sostoyanie_UES2;
       }
     if(i6>=0x1000001&&i6<0x2000000) {
       flag_zagruzki_zagolovka=0;
       flag_zagruzki_dliny_massiva=1;
       nomer_massiva=i6-0x1000000;
       flag_zagruzki_iera=1;
       goto load_sostoyanie_UES3;
       }
     if(i6>=0x3000001&&i6<0x4000000) {
       flag_zagruzki_zagolovka=0;
       flag_zagruzki_dliny_massiva=1;
       nomer_massiva=i6-0x3000000;
       flag_zagruzki_vetveq=1;
       goto load_sostoyanie_UES3;
       }
     if(i6>=0x75000001&&i6<0x76000000) {
       flag_zagruzki_zagolovka=0;
       flag_zagruzki_dliny_massiva=1;
       nomer_massiva=i6-0x75000000;
       flag_zagruzki_massivov=1;
       goto load_sostoyanie_UES3;
       }
     return 110100435;// 110100435 = ошибка в заголовке
     }
   if(flag_zagruzki_dliny_massiva==1) {
     flag_zagruzki_dliny_massiva=0;
     dlina_massiva=i6;
     nomer_v_massive=1;
     goto load_sostoyanie_UES3;
     }
   if(flag_zagruzki_iera==1) {
     massiv_iera[nomer_massiva][nomer_v_massive]=i6;
     nomer_v_massive++;
     if(nomer_v_massive>=dlina_massiva) {
       flag_zagruzki_iera=0;
       massiv_iera[nomer_massiva][0]=dlina_massiva;
       flag_zagruzki_zagolovka=1;
       }
     goto load_sostoyanie_UES3;
     }
   if(flag_zagruzki_vetveq==1) {
     massiv_vetveq[nomer_massiva][nomer_v_massive]=i6;
     nomer_v_massive++;
     if(nomer_v_massive>=dlina_massiva) {
       flag_zagruzki_vetveq=0;
       massiv_vetveq[nomer_massiva][0]=dlina_massiva;
       flag_zagruzki_zagolovka=1;
       }
     goto load_sostoyanie_UES3;
     }
   if(flag_zagruzki_massivov==1) {
     BZ100000[nomer_massiva][nomer_v_massive]=i6;
     nomer_v_massive++;
     if(nomer_v_massive>=dlina_massiva) {
       flag_zagruzki_massivov=0;
       BZ100000[nomer_massiva][0]=dlina_massiva;
       flag_zagruzki_zagolovka=1;
       }
     goto load_sostoyanie_UES3;
     }
   ;
load_sostoyanie_UES3:
   nomer_v_buffer++;
   if(nomer_v_buffer<i5)
     goto load_sostoyanie_UES1;
   }
load_sostoyanie_UES2:
 close(lst1);
 return 110100436;// 110100436 = нормальный выход
 }

// перенос в массив начала и длины текста ограниченного справа разделительными
// символами, перенос в отдельный массив причастных к этому разделительных символов
// в формате номер эквивалентной строки, далее тип разделительного символа
unsigned int index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol(
  unsigned int *massiv4, unsigned int *massiv3, unsigned int *massiv2,
  unsigned int *massiv1) {
//massiv1=text
//massiv2=specsimvol
//massiv3=index_start_i_dlina_text
//massiv4=index_nomer_stroki_i_specsimvol
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10;
 unsigned int f_text=0;
 unsigned int f_massiv_end=0;
 i1=massiv1[0];
 i2=1;//по тексту
 i3=massiv2[0];
 i4=1;//по спецсимволам
 i5=1;//по индексному массиву текста(старт строки и длина)
 i10=0;//длина строки
 i6=1;//по индексному массиву спецсимволов(номер строки и спецсимвол)
 i7=0;//номер строки
 if(i1<=i2)
   goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol1;
 if(i3<=i4)
   goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol1;
index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol2:
 i8=massiv1[i2];
index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol3:
 i9=massiv2[i4];
 if(i8==i9)
   goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol4;
 i4++;
 if(i4<i3)
   goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol3;
//символ обычный
 if(f_text==0) {
   f_text=1;
   i7++;
   massiv3[i5]=i2;
   i5++;
   }
 goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol6;
//встретился спецсимвол или конец массива
index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol4:
 if(f_text==1) {
   f_text=0;
   massiv3[i5]=i10;
   i10=0;
   i5++;
   }
 massiv4[i6]=i7;
 i6++;
 if(f_massiv_end==1)
   i4=0;
 massiv4[i6]=i4;
 i6++;
 if(f_massiv_end==1)
   goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol1;
//промоталась вся таблица спецсимволов значит встретился обычный символ
index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol6:
 i4=1;
index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol5:
 if(f_text==1)
   i10++;
 i2++; 
 if(i2>=i1) {
   f_massiv_end=1;
   goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol4;
   }
 goto index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol2;
index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol1:
 massiv3[0]=i5;
 massiv4[0]=i6;
 return 110100437;// 110100437 = нормальный выход
 }

//вырезание в массив текста массива со стартовой позиции по определенную длину
unsigned int vyrezalka_so_start_po_dlinu(unsigned int *massiv2, unsigned int *massiv1,
  unsigned int start, unsigned int dlina) {
 unsigned int i1,i2,i3,i4,i5;
 i1=massiv1[0];
 i2=start;
 i3=1;
 if(i2>=i1)
   return 110100438;// 110100438 = некорректный старт
 i5=start+dlina;
 if(i5>i1)
   return 110100439;// 110100439 = некорректная длина
vyrezalka_so_start_po_dlinu1:
 if(i2>=i5)
   goto vyrezalka_so_start_po_dlinu2;
 i4=massiv1[i2];
 massiv2[i3]=i4;
 i2++;
 i3++;
 goto vyrezalka_so_start_po_dlinu1;
vyrezalka_so_start_po_dlinu2:
 massiv2[0]=i3;
 return 110100440;// 110100440 = нормальный выход
 }

//инициализация и освобождение всех массивов BZ100000
unsigned int init_i_clean_BZ100000(void) {
 unsigned int i1;
 for(i1=0;i1<=max_kolichestvo_BZ100000;i1++) 
   BZ100000_be_RAM[i1]=0;
 return 110100441;// 110100441 = нормальный выход
 }

//выделение номера первого свободного массива BZ100000, если нет тогда возвращает 0
unsigned int vydel_1_free_BZ100000(void) {
 unsigned int i1;
 for(i1=1;i1<=max_kolichestvo_BZ100000;i1++) 
   if(BZ100000_be_RAM[i1]==0)
     return i1;
 return 0;// 0 = нет свободного массива BZ100000
 }

//=====
//загрузка в иерархию текста из массива BZ100000
unsigned int zagruzka_text_iera_BZ100000(unsigned int iera,
  unsigned int object, unsigned int massiv) {
unsigned int chislo_zapiseq1;
unsigned int i1,i2;
unsigned int len1,len2;
 printf("a1 iera=%i\n",iera);
 if(iera<0x1000000||iera>=0x2000000)
   return 110100444;// 110100444 = это не иерархия
 printf("a2 massiv=%i\n",massiv);
 if(massiv<0x75000001||massiv>0x76000000)
   return 110100445;// 110100445 = это не BZ100000 массив
 printf("a3\n");
 iera=iera-0x1000000;
 massiv=massiv-0x75000000;
 printf("a1 iera=%i\n",iera);
 printf("a2 massiv=%i\n",massiv);
 len1=massiv_iera[iera][0];
 len2=BZ100000[massiv][0];
 chislo_zapiseq1=massiv_iera[iera][2];
 i1=len1;
 massiv_iera[iera][i1]=0x26001042;
 i1++;
 massiv_iera[iera][i1]=object;
 i1++;
 massiv_iera[iera][i1]=1;
 i1++;
 massiv_iera[iera][i1]=len2-1;
 i1++;
 for(i2=1;i2<len2;i2++) {
   massiv_iera[iera][i1]=BZ100000[massiv][i2];
   i1++;
   }
 massiv_iera[iera][2]=chislo_zapiseq1+1;
 massiv_iera[iera][0]=i1;
 return 110100446;// 110100446 = нормальный выход
 }

//прописка из файла в имена объектов и номера объектов
unsigned int set_from_file_name_object_i_nomer_object(void) {
 unsigned int i1,i2,i3,i4,i5,i6,i7;
 unsigned int massiv1[1000];
 unsigned int massiv2[1000];
 unsigned int massiv3[1000];
 unsigned int massiv4[1000];
 i2=0;
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_path[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_spec_path[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 for(i1=0;i1<max_dlina_dlinnogo_text;i1++) {
   i3=BZ_file_name_object_i_nomer_object[i1];
   if(i3==0)
     break;
   obschee_name_file[i2]=i3;
   i2++;
   }
 obschee_name_file[i2]=0;
 i1=vydel_1_free_BZ100000();
 if(i1==0)
   return 110100442;// 110100442 = нет свободного массива
 BZ100000_be_RAM[i1]=1;//массив для всего конфигурационного файла
 copy_charfile2intmassiv(BZ100000[i1], obschee_name_file);//загрузка из файла в массив

 i2=vydel_1_free_BZ100000();
 if(i2==0)
   return 110100443;// 110100443 = нет свободного массива
 BZ100000_be_RAM[i2]=1;//массив для спецсимвола перевод строки
 BZ100000[i2][0]=2;
 BZ100000[i2][1]=10;//массив со спецсимволом перевода строки
 i3=vydel_1_free_BZ100000();
 if(i3==0)
   return 110100444;// 110100444 = нет свободного массива
 BZ100000_be_RAM[i3]=1;//массив для индексов адреса и длин строк
 i4=vydel_1_free_BZ100000();
 if(i4==0)
   return 110100445;// 110100445 = нет свободного массива
 BZ100000_be_RAM[i4]=1;//массив для индексов адресов спецсимволов
 index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol(BZ100000[i4],
  BZ100000[i3],BZ100000[i2],BZ100000[i1]);
 i5=BZ100000[i3][0];

 i6=vydel_1_free_BZ100000();
 if(i6==0)
   return 110100446;// 110100446 = нет свободного массива
 i7=1;
 printf("i5=%i\n",i5);
 printf("i6=%i\n",i6);
//=====

 printf("i7=%i\n",i7);
 printf("BZ100000[i3][i6]\n");
 for(i6=0;i6<i5;i6++) {
   printf("%i\n",BZ100000[i3][i6]);
   }
 printf("ccccc\n");

 return 110100443;// 110100443 = нормальный выход
 }

// печать на экран записи иерархии по адресу записи
unsigned int print_zapis(unsigned int iera, unsigned int adres_zapis) {
 unsigned int i1,i2,i3,i4,i5,i6;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100461;// 110100461 = это не иерархия
 iera=iera-0x1000000;
 i1=massiv_iera[iera][adres_zapis];
 adres_zapis++;
 i2=massiv_iera[iera][adres_zapis];
 adres_zapis++;
 i3=massiv_iera[iera][adres_zapis];
 adres_zapis++;
 i4=massiv_iera[iera][adres_zapis];
 adres_zapis++;
 i5=adres_zapis+i4;
 printf("%i %i %i %i  ",i1,i2,i3,i4);
 for(i6=adres_zapis;i6<i5;i6++)
   printf("%i ",massiv_iera[iera][i6]);
 printf("\n");
 return 110100462;
 }

// печать на экран иерархии
unsigned int print_iera(unsigned int iera) {
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10,i11,i12;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100463;// 110100463 = это не иерархия
 i1=iera;
 iera=iera-0x1000000;
 i2=massiv_iera[iera][0];
 i3=massiv_iera[iera][1];
 i4=massiv_iera[iera][2];
 i5=massiv_iera[iera][3];
 i6=massiv_iera[iera][4];
 i7=massiv_iera[iera][5];
 printf("%i %i %i  %i %i %i\n",i2,i3,i4,i5,i6,i7);
 i9=6;
 for(i8=1;i8<=i4;i8++) {
   i10=i9+3;
   i11=massiv_iera[iera][i10];
   i12=i9+3+i11+1;
   if(i12>i2) {
     return 110100464;// 110100464 = записей больше чем длина массива
     }
   print_zapis(i1,i9);
   i9=i12;
   }
 return 110100465;
 }

// загрузка записи текущей иерархии цифрами
unsigned int zagruzka_zapis_int(unsigned int iera) {
 unsigned int i1,i2,i3,i4,i5,i6;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100466;// 110100466 = это не иерархия
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 printf("characteristic:");
 scanf("%s",&massiv_system_text_buffer);
 copy_text2intmassiv(massiv_system_int_buffer, massiv_system_text_buffer);
 i2=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
 if(i2==0)
   return 110100467;// 0 - exit
 massiv_iera[iera][i1]=i2;
 i1++;
 printf("pervichnyq_obj:");
 scanf("%s",&massiv_system_text_buffer);
 copy_text2intmassiv(massiv_system_int_buffer, massiv_system_text_buffer);
 i2=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
 if(i2==0)
   return 110100467;// 0 - exit
 massiv_iera[iera][i1]=i2;
 i1++;
 printf("cena_svyazi   :");
 scanf("%s",&massiv_system_text_buffer);
 copy_text2intmassiv(massiv_system_int_buffer, massiv_system_text_buffer);
 i2=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
 if(i2==0)
   return 110100467;// 0 - exit
 massiv_iera[iera][i1]=i2;
 i1++;
 printf("kolichestvo_vt:");
 scanf("%s",&massiv_system_text_buffer);
 copy_text2intmassiv(massiv_system_int_buffer, massiv_system_text_buffer);
 i2=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
 if(i2==0)
   return 110100467;// 0 - exit
 massiv_iera[iera][i1]=i2;
 i1++;
 for(i3=0;i3<i2;i3++) {
   printf("vt_object(%i):",i3);
   scanf("%s",&massiv_system_text_buffer);
   copy_text2intmassiv(massiv_system_int_buffer, massiv_system_text_buffer);
   i4=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
   if(i4==0)
     return 110100467;// 0 - exit
   massiv_iera[iera][i1+i3]=i4;
   }
 massiv_iera[iera][1]=zanyataya_iera;
 i5=massiv_iera[iera][2];
 i5++;
 massiv_iera[iera][2]=i5;
 massiv_iera[iera][0]=i1+i3;
 return 110100468;
 }


//======
// загрузка записи текущей иерархии банальным текстом
unsigned int zagruzka_zapis_text(unsigned int iera) {
 unsigned int i1,i2,i3,i4,i5,i6,i7;
 if(iera<0x1000000||iera>=0x2000000)
   return 110100469;// 110100469 = это не иерархия
 iera=iera-0x1000000;
 i1=massiv_iera[iera][0];
 massiv_iera[iera][i1]=text_banalnoe_format_koi8r;
 printf("pervichnyq_obj:");
 scanf("%s",&massiv_system_text_buffer);
 copy_text2intmassiv(massiv_system_int_buffer, massiv_system_text_buffer);
 i2=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
 printf("i2=%i\n",i2);
 if(i2==0)
   return 110100470;// 0 - exit
 i1++;
 massiv_iera[iera][i1]=i2;
 i1++;
 massiv_iera[iera][i1]=1;
 i1++;
 printf("enter text:");
 zapolnenie_massiva_int_textom_keyboard(massiv_system_int_buffer);
 i3=massiv_system_int_buffer[0]-1;
 massiv_iera[iera][i1]=i3;
 i1++;
 i4=i1+i3;
 if(i4>max_dlina_iera)
   return 110100471;// 110100471 = выход за длину иерархии
 i5=1;
 for(i6=i1;i6<i4;i6++) {
   massiv_iera[iera][i6]=massiv_system_int_buffer[i5]+text_koi8r_page;
   i5++;
   }
 massiv_iera[iera][0]=i4;
 massiv_iera[iera][1]=zanyataya_iera;
 i7=massiv_iera[iera][2];
 i7++;
 massiv_iera[iera][2]=i7;
 return 110100472;
 }



//перевод текста из массива BZ100000 в иерархию по именам объектов
//unsigned int BZ100000[100][100000];// массивы для харакеристик длиной 99 999 слов, 0 адрес это длина

// трансляция буфера клавиатуры в команду монитора
unsigned int monitor_massiv_int_to_monitor_command(unsigned int *massiv) {
 unsigned int i1;
 if(massiv[0]==0||massiv[0]==1)
   return 110100424;// некорректный массив
 if(massiv[1]=='c') {// c - пустая команда
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   return 0x26000701;
   }
 if(massiv[1]=='h') {// h - вывод справочной информации общее
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   return 0x26000711;
   }
 if(massiv[1]=='r') {// r - запуск выполнения задания в иерархии первичного алгоритма Sh
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   return 0x26000721;
   }
 if(massiv[1]=='s') {// s - остановка выполнения задания в иерархии первичного алгоритма
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   return 0x26000731;
   }
 if(massiv[1]=='q') {// q - выход из программы УЭС без сохранения результатов в иерархиях, ветвях и массивах
   system_rejim1[0]=UES_rejim_hard_exit;//режим работы УЭС типа жесткий выход с закрытием процессов и сокитов обмена данными
   return 0x26000741;
   }
 if(massiv[1]=='w') {// w - выход из программы УЭС с сохранением результатов в иерархиях, ветвях и массивах
   system_rejim1[0]=UES_rejim_hard_exit;//режим работы УЭС типа мягкий выход с закрытием процессов и сокитов обмена данными
   return 0x26000751;
   }
 if(massiv[1]=='l') {// l - загрузка
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   if(massiv[0]<3)
     return 0x26000701;//ошибка команды монитора
   if(massiv[2]=='c') {// lс - загрузка команды
     return 0x26000762;
     }
   if(massiv[2]=='f') {// lf - загрузка файла в массив
     return 0x26000763;
     }
   if(massiv[2]=='s') {// ls - загрузка результатов в иерархиях, ветвях и массивах
     return 0x26000764;
     }
   return 0x26000701;
   }
 if(massiv[1]=='t') {// t - обучение информации общее или трансляция
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   if(massiv[0]<4)
     return 0x26000701;//ошибка команды монитора
   if(massiv[2]=='t') {
     if(massiv[3]=='k') {// ttk - обучение текстовым фактам с клавиатуры
       return 0x26000772;
       }
     if(massiv[3]=='f') {// ttf<file> - обучение текстовым фактам из файла
       if(massiv[0]>4)
         return 0x26000701;
       return 0x26000773;
       }
     return 0x26000701;
     }
   if(massiv[2]=='i') {
     if(massiv[3]=='k') {
       if(massiv[3]=='d') {// tik - обучение int фактам с клавиатуры десятичное
         return 0x26000774;
         }
       if(massiv[3]=='h') {// tik - обучение int фактам с клавиатуры шестнадцатиричное
         return 0x26000775;
         }
       }
     if(massiv[3]=='f') {// tif<file> - обучение int фактам из файла
       if(massiv[0]<5)
         return 0x26000701;
       return 0x26000776;
       }
     if(massiv[3]=='i') {// tii<nomer> - обучение int фактам из иерархии
       if(massiv[0]<5)
         return 0x26000701;
       return 0x26000777;
       }
     return 0x26000701;
     }
   if(massiv[2]=='r') {
     if(massiv[3]=='h') {// trh - трансляция шестнадцатиричной цифры в десятичную
       return 0x26000778;
       }
     if(massiv[3]=='d') {// trd - трансляция десятичной цифры в шестнадцатиричную
       return 0x26000779;
       }
     return 0x26000701;
     }
   return 0x26000701;
   }
 if(massiv[1]=='v') {// v - вывод рабочей информации общее
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   if(massiv[2]=='i') {// vi  - вывод текущей иерархии (2017-05-30-04-17)
     return 0x26000802;
     }
   if(massiv[2]=='m') {
     if(massiv[3]=='i') {// vmi  - вывод текущего системного массива int
       return 0x26000803;
       }
     if(massiv[3]=='t') {// vmt  - вывод текущего системного массива text
       return 0x26000804;
       }
     }
   if(massiv[2]=='a') {// va<nomer>  - вывод алгоритма <nomer>
     if(massiv[0]==3)
       return 0x26000805;
     if(massiv[0]<4)
       return 0x2000701;
     return 0x26000806;
     }
   if(massiv[2]=='c') {
     if(massiv[3]=='i') {// vсi  - вывод каталога иерархий
       return 0x26000807;
       }
     if(massiv[3]=='v') {// vсv  - вывод каталога ветвей
       return 0x26000808;
       }
     return 0x26000701;
     }
   if(massiv[2]=='i') {// vi<nomer>  - вывод иерархии <nomer>
     if(massiv[0]<4)
       return 0x26000701;
     return 0x26000809;
     }
   if(massiv[2]=='o') {// vo<nomer> - вывод объекта <nomer>
     if(massiv[0]<4)
       return 0x26000701;
     return 0x26000810;
     } 
   if(massiv[2]=='n') {
     if(massiv[3]=='i') {// vni<name> - вывод иерархии с именем и её имя
       if(massiv[0]<5)
         return 0x26000701;
       return 0x26000811;
       }
     if(massiv[3]=='o') {// vno<name> - вывод объекта с именем и его имя
       if(massiv[0]<5)
         return 0x26000701;
       return 0x26000812;
       }
     return 0x26000701;
     }
   return 0x26000701;
   } 
 if(massiv[1]=='i') {// i - загрузка рабочей информации общее 
   system_rejim1[0]=UES_rejim_prostoq;//режим работы УЭС типа простой
   if(massiv[2]=='z')
     if(massiv[3]=='k') {
       if(massiv[4]=='h') {// izkh - загрузка текущей иерархии записи цифрами (2017-05-30-06-11)
         return 0x26000813;
         }
       if(massiv[4]=='t') {// izkt - загрузка текущей иерархии записи банальным текстом (2017-05-30-06-22)
         return 0x26000814;
         }
       }
     }

 return 0x26000701;
 }

// выполнение команды монитора с использованием буфера клавиатуры
unsigned int monitor_massiv_int_to_monitor_command(unsigned int command,
  unsigned int *massiv) {
 unsigned int i1;
 if(command==0x26000701)// c - пустая команда
   return 110100425;//нормальный выход
 if(command==0x26000711) {// h - вывод справочной информации общее
   monitor_help_info(massiv_system_text_buffer);
   preobrazovanie_massiva_char_v_massiv_int(massiv_system_text_buffer, massiv_system_int_buffer);
   vyvod_massiva_int(massiv_system_int_buffer);
   return 110100425;
   }
 if(command==0x26000721)//r - запуск выполнения задания в иерархии первичного алгоритма Shell для УЭС
   return 110100425;
 if(command==0x26000731)//s - остановка выполнения задания в иерархии первичного алгоритма Shell для УЭС
   return 110100425;
 if(command==0x26000741)//q - выход из программы УЭС без сохранения результатов в оперативных иерархиях
   return 110100425;
 if(command==0x26000751) {//w - выход из программы УЭС с сохранением результатов в оперативных иерархиях(сохранение рабочего состояния УЭС)
   save_sostoyanie_UES();
   return 110100425;
   }
 if(command==0x26000761)//l - загрузка
   return 110100425;
 if(command==0x26000762)//lс - загрузка команды
   return 110100425;
 if(command==0x26000763)//lf - загрузка файла в массив
   return 110100425;
 if(command==0x26000764) {//ls - загрузка результатов в иерархиях, ветвях и массивах
   load_sostoyanie_UES();
//=====

   return 110100425;
   }
 if(command==0x26000771)//t - обучение информации общее или трансляция
   return 110100425;
 if(command==0x26000772)//ttk - обучение текстовым фактам с клавиатуры
   return 110100425;
 if(command==0x26000773) {//ttf<file> - обучение текстовым фактам из файла
   perevod_int_massiva_pozition_v_text(massiv,4,massiv_system_text_buffer);
   i1=perevod_text_v_namepath_i_namefile(massiv_system_text_buffer,now_path,now_file);
   if(i1!=110100422)
     return 110100426;//ошибка определения файла
   return 110100425;
   }
 if(command==0x26000774)//tikd - обучение int фактам с клавиатуры десятичное
   return 110100425;
 if(command==0x26000775)//tikh - обучение int фактам с клавиатуры шестнадцатиричное
    return 110100425;
 if(command==0x26000776) {//tif<file> - обучение int фактам из файла
   perevod_int_massiva_pozition_v_text(massiv,4,massiv_system_text_buffer);
   i1=perevod_text_v_namepath_i_namefile(massiv_system_text_buffer,now_path,now_file);
   if(i1!=110100422)
     return 110100426;//ошибка определения файла
   }
 if(command==0x26000777) {//tii<nomer> - обучение int фактам из иерархии
   ostatok_massiva_int_right(massiv,4,massiv_system_int_buffer);
   system_int=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
   return 110100425;
   }
 if(command==0x26000778) {//trh - трансляция шестнадцатиричной цифры в десятичную
   ostatok_massiva_int_right(massiv,4,massiv_system_int_buffer);
   for(i1=0;i1<massiv_system_int_buffer[0];i1++)
   system_int=perevod_text_16cifry_v_int_to_return(massiv_system_int_buffer);
   return 110100425;
   }
 if(command==0x26000779) {//trd - трансляция десятичной цифры в шестнадцатиричную
   ostatok_massiva_int_right(massiv,4,massiv_system_int_buffer);
   system_int=perevod_text_10cifry_v_int_to_return(massiv_system_int_buffer);
   perevod_int_cifry_v_int_16rich_massiv(massiv_system_int_buffer,system_int);
   return 110100425;
   }
 if(command==0x26000801)//v - вывод рабочей информации общее
   return 110100425;
 if(command==0x26000802) {//vi  - вывод текущей иерархии (2017-05-30-04-17)
   print_iera(n03);
   return 110100425;
   }
 if(command==0x26000803)//vmi  - вывод текущего системного массива int
   return 110100425;
 if(command==0x26000804)//vmt  - вывод текущего системного массива text
   return 110100425;
 if(command==0x26000805)//va  - вывод первичного алгоритма
   return 110100425;
 if(command==0x26000806) {//va<nomer>  - вывод алгоритма <nomer>
   perevod_int_massiva_pozition_v_text(massiv,3,massiv_system_text_buffer);
   return 110100425;
   }
 if(command==0x26000807)//vсi  - вывод каталога иерархий
   return 110100425;
 if(command==0x26000808)//vсv  - вывод каталога ветвей
   return 110100425;
 if(command==0x26000809) {//vi<nomer>  - вывод иерархии <nomer>
   perevod_int_massiva_pozition_v_text(massiv,3,massiv_system_text_buffer);
   return 110100425;
   }
 if(command==0x26000810) {//vo<nomer> - вывод объекта <nomer>
   perevod_int_massiva_pozition_v_text(massiv,3,massiv_system_text_buffer);
   return 110100425;
   }
 if(command==0x26000811) {//vni<name> - вывод иерархии с именем и её имя
   perevod_int_massiva_pozition_v_text(massiv,4,massiv_system_text_buffer);
   return 110100425;
   }
 if(command==0x26000812) {//vno<name> - вывод объекта с именем и его имя
   perevod_int_massiva_pozition_v_text(massiv,4,massiv_system_text_buffer);
   return 110100425;
   }
 if(command==0x26000813) {//izkh - загрузка текущей иерархии записи цифрами (2017-05-30-06-11)
   zagruzka_zapis_int(n03);
   return 110100425;
   }
 if(command==0x26000814) {//izkt - загрузка текущей иерархии записи банальным текстом (2017-05-30-06-22)
   zagruzka_zapis_text(n03);
   return 110100425;
   }
 }

// ==100==
/****************************************************************************/
/*+------------------------------------------------------------------------+*/  
/*|                 Универсальная Экспертная Система                       |*/  
/*+------------------------------------------------------------------------+*/  
/****************************************************************************/
// основная программа Универсальной Экспертной Системы
int main(int argc, char ** argv) {
 unsigned int i;
 unsigned int i1,i2,i3,i4,i5,i6,i7,i8,i9,i10;

 init_i_clean_BZ100000();

 printf("a1\n");
 for(i1=0;i1<=max_chislo_iera;i1++)
//======
   massiv_iera[i1][1]=svobodnaya_iera;
 printf("a2\n");
 for(i1=0;i1<=max_chislo_vetveq;i1++)
   massiv_vetveq[i1][1]=svobodnaya_vetv;
 printf("a3\n");
 for(i1=0;i1<=max_kolichestvo_BZ100000;i1++)
   BZ100000_be_RAM[i1]=0;
 printf("a4\n");

 printf("zyx1\n");
// set_from_file_name_object_i_nomer_object();
 printf("zyx2\n");

 int massivtest1[100]={9,1,2,3,4,5,6,7,8};
 for(i1=0;i1<9;i1++)
   massiv_iera[1][i1]=massivtest1[i1];
 int massivtest2[100]={9,11,12,13,14,15,16,17,18};
 for(i1=0;i1<9;i1++)
   massiv_vetveq[1][i1]=massivtest2[i1];
// int massivtest3[100]={9,21,22,23,24,25,26,27,28,};
 int massivtest3[100]={20,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
 for(i1=0;i1<20;i1++) {
   BZ100000[1][i1]=massivtest3[i1];
   BZ100000[2][i1]=massivtest3[i1];
   BZ100000[3][i1]=massivtest3[i1];
   BZ100000[4][i1]=massivtest3[i1];
   BZ100000[5][i1]=massivtest3[i1];
   BZ100000[6][i1]=massivtest3[i1];
   BZ100000[7][i1]=massivtest3[i1];
   BZ100000[8][i1]=massivtest3[i1];
   BZ100000[9][i1]=massivtest3[i1];
   BZ100000[10][i1]=massivtest3[i1];
   BZ100000[11][i1]=massivtest3[i1];
   BZ100000[12][i1]=massivtest3[i1];
   BZ100000[13][i1]=massivtest3[i1];
   BZ100000[14][i1]=massivtest3[i1];
   BZ100000[15][i1]=massivtest3[i1];
   BZ100000[16][i1]=massivtest3[i1];
   BZ100000[17][i1]=massivtest3[i1];
   BZ100000[18][i1]=massivtest3[i1];
   BZ100000[19][i1]=massivtest3[i1];
   BZ100000[20][i1]=massivtest3[i1];
   }
 BZ100000_be_RAM[1]=1;
 BZ100000_be_RAM[2]=1;
 BZ100000_be_RAM[3]=1;
 BZ100000_be_RAM[4]=1;
 BZ100000_be_RAM[5]=1;
 BZ100000_be_RAM[6]=1;
 BZ100000_be_RAM[7]=1;
 BZ100000_be_RAM[8]=1;
 BZ100000_be_RAM[9]=1;
 BZ100000_be_RAM[10]=1;
 BZ100000_be_RAM[11]=1;
 BZ100000_be_RAM[12]=1;
 BZ100000_be_RAM[13]=1;
 BZ100000_be_RAM[14]=1;
 BZ100000_be_RAM[15]=1;
 BZ100000_be_RAM[16]=1;
 BZ100000_be_RAM[17]=1;
 BZ100000_be_RAM[18]=1;
 BZ100000_be_RAM[19]=1;
 BZ100000_be_RAM[20]=1;
 int nomer_massiva=1;

 massiv_iera[1][0]=28;
 massiv_iera[1][1]=1;
 massiv_iera[1][2]=3;
 massiv_iera[1][3]=11;
 massiv_iera[1][4]=12;
 massiv_iera[1][5]=13;
 massiv_iera[1][6]=100;
 massiv_iera[1][7]=101;
 massiv_iera[1][8]=102;
 massiv_iera[1][9]=5;
 massiv_iera[1][10]=21;
 massiv_iera[1][11]=22;
 massiv_iera[1][12]=23;
 massiv_iera[1][13]=24;
 massiv_iera[1][14]=25;
 massiv_iera[1][15]=103;
 massiv_iera[1][16]=104;
 massiv_iera[1][17]=105;
 massiv_iera[1][18]=2;
 massiv_iera[1][19]=31;
 massiv_iera[1][20]=32;
 massiv_iera[1][21]=106;
 massiv_iera[1][22]=107;
 massiv_iera[1][23]=108;
 massiv_iera[1][24]=3;
 massiv_iera[1][25]=41;
 massiv_iera[1][26]=42;
 massiv_iera[1][27]=43;
 
 printf("BZ100000_be_RAM %i = %i\n",nomer_massiva,BZ100000_be_RAM[nomer_massiva]);

//=====
 unsigned int massivtest4[100]={11,'-','-','0',' ','1','-','t','e','s','t'};
 unsigned int massivtest5[100]={2,'-'};
 unsigned int massivtest6[100];
 unsigned int massivtest7[100];
// index_kak_start_i_dlina_v_massivy_indextext_i_specsimvol(massivtest7,
//  massivtest6, massivtest5, massivtest4);

 printf("massivtest6\n");
 for(i1=0;i1<10;i1++)
  printf("%i \n",massivtest6[i1]);
 printf("massivtest7\n");
 for(i1=0;i1<10;i1++)
  printf("%i \n",massivtest7[i1]);
 printf("\n");

 unsigned int massivtest8[100]={5,10,20,30,40};
// for(i1=0;i1<5;i1++)
//   BZ100000[2][i1]=massivtest8[i1];
// init_iera(0x1000020,7,8,9);
// zagruzka_text_iera_BZ100000(0x1000020,88,0x75000002);
//  printf("iera20:\n");

// for(i1=0;i1<15;i1++)
//  printf("%i %i\n",i1,massiv_iera[0x20][i1]);

 print_16_int(4100000000);

// return 0;
 
 (void)signal(SIGUSR1, system_flag1);
// char mass1[]="./a7\0";
// char path1[10];
// char file1[10];
// i1= perevod_text_v_namepath_i_namefile(mass1,path1,file1);
//  printf("i1=%i\n",i1); 
//  printf("%s\n",mass1); 
//  printf("%s\n",path1); 
//  printf("%s\n",file1); 
  
// unsigned int mass2[4]={4,49,49,49};
//  i1=perevod_text_10cifry_v_int_to_return(mass2);
//  printf("i1=%i\n",i1); 
// unsigned int mass3[4]={4,49,48,49};
//  i1=perevod_text_16cifry_v_int_to_return(mass3);
//  printf("i1=%i\n",i1); 

// zapolnenie_massiva_int_textom_keyboard(massiv_int_keyboard);
// for(i=0;i<10;i++)
//   printf("%i %i %c\n",i,massiv_int_keyboard[i],massiv_int_keyboard[i]); 

// printf("\n"); 
// i1=258;
// perevod_int_cifry_v_int_16rich_massiv(massiv_int_keyboard,i1);
// for(i=0;i<10;i++)
//   printf("%i %i %c\n",i,massiv_int_keyboard[i],massiv_int_keyboard[i]); 


 pid_t pid;
 pid = fork();
 if(pid==-1) {
   /* Аварийное завершение */
   perror("fork failed");
   exit(1);
   }
 if(pid==0) {
   /* Дочерний процесс */
   int sock1;
   struct sockaddr_in addr;
   sock1 = socket(AF_INET, SOCK_STREAM, 0);
   if(sock1 < 0) {
     perror("socket");
     exit(1);
     }
   addr.sin_family = AF_INET;
   addr.sin_port = htons(31137); // или любой другой порт...
   addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
   while(connect(sock1, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
     sleep(1);
     }
   while(1) {
     if(system_rejim1[0]==UES_rejim_prostoq) {
//       scanf("%s",&keyboardline);
       zapolnenie_massiva_int_textom_keyboard(massiv_int_keyboard);//чтение с клавиатуры
       system_rejim1[0]=UES_rejim_keyboard_buffer_full;
       kill(getppid(), SIGUSR1);
//       send(sock1, keyboardline, sizeof(keyboardline), 0);
       send(sock1, massiv_int_keyboard, 12345, 0);
       recv(sock1, system_rejim1, 4 , 0);
       }
     sleep(1);
     printf("system_rejim1_1=%i\n",system_rejim1[0]);
     if(system_rejim1[0]==UES_rejim_hard_exit) {
     printf("system_rejim1_1k1=%i\n",system_rejim1[0]);
       close(sock1);
     printf("system_rejim1_1k2=%i\n",system_rejim1[0]);
       exit(0);
       }
     }
   }
 struct sockaddr_in addr;
 listener = socket(AF_INET, SOCK_STREAM, 0);
 if(listener < 0) {
   exit(1);
   }
 addr.sin_family = AF_INET;
 addr.sin_port = htons(31137);
 addr.sin_addr.s_addr = htonl(INADDR_ANY);
 if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
   perror("bind");
   exit(2);
   }
 listen(listener, 1);
 sock2 = accept(listener, NULL, NULL);
 if(sock2 < 0) {
   perror("accept");
   exit(3);
   }
 while(1) {// ***** ГЛАВНЫЙ ЦИКЛ ПРОГРАММЫ, В НЕЙ НАДО РАСПОЛОГАТЬ МЕНЕДЖЕР ЗАДАЧ
   if(system_rejim1[0]==UES_rejim_keyboard_buffer_full) {
//     bytes_read = recv(sock2, keyboardline, 1000, 0);
     bytes_read = recv(sock2, massiv_int_keyboard, 12345, 0);
//     keyboardline[bytes_read]=(unsigned char)0;
//     printf("keyboardline=%s\n",keyboardline);
     for(i=0;i<10;i++)
       printf("%i %i %c\n",i,massiv_int_keyboard[i],massiv_int_keyboard[i]); 
     system_rejim1[0]=UES_rejim_prostoq;
     monitor_command_now=monitor_massiv_int_to_monitor_command(massiv_int_keyboard);
     monitor_massiv_int_to_monitor_command(monitor_command_now,massiv_int_keyboard);
//=====
     printf("command = %i\n", monitor_command_now);
     if(monitor_command_now==0x26000701) {
       printf("c - пустая команда\n");
       }
     if(monitor_command_now==0x26000711) {//2 h - вывод справочной информации общее
       printf("h - вывод справочной информации общее\n");
       }
     if(monitor_command_now==0x26000721) {
       printf("r - запуск выполнения задания в иерархии первичного алгоритма Shell для УЭС\n");
       }
     if(monitor_command_now==0x26000731) {
       printf("s - остановка выполнения задания в иерархии первичного алгоритма Shell для УЭС\n");
       }
     if(monitor_command_now==0x26000741) {
       printf("q - выход из программы УЭС без сохранения результатов в иерархиях\n");
       }
     if(monitor_command_now==0x26000751) {
       printf("w - выход из программы УЭС с сохранением результатов в иерархиях\n");
       }
     if(monitor_command_now==0x26000761) {
       printf("l - загрузка\n");
       }
     if(monitor_command_now==0x26000762) {
       printf("lс - загрузка команды\n");
       }
     if(monitor_command_now==0x26000763) {
       printf("lf - загрузка файла в массив\n");
       }
     if(monitor_command_now==0x26000764) {
       printf("ls - загрузка результатов в иерархиях, ветвях и массивах\n");
       }
     if(monitor_command_now==0x26000771) {
       printf("t - обучение информации общее или трансляция\n");
       }
     if(monitor_command_now==0x26000772) {
       printf("ttk - обучение текстовым фактам с клавиатуры\n");
       }
     if(monitor_command_now==0x26000773) {
       printf("ttf<file> - обучение текстовым фактам из файла\n");
       }
     if(monitor_command_now==0x26000774) {
       printf("tikd - обучение int фактам с клавитауры шестнадцатиричное\n");
       }
     if(monitor_command_now==0x26000775) {
       printf("tikh - обучение int фактам с клавитауры шестнадцатиричное\n");
       }
     if(monitor_command_now==0x26000776) {
       printf("tif<file> - обучение int фактам из файла\n");
       }
     if(monitor_command_now==0x26000777) {
       printf("tii<nomer> - обучение int фактам из иерархии\n");
       }
     if(monitor_command_now==0x26000778) {
       printf("trh - трансляция шестнадцатиричной цифры в десятичную\n");
       }
     if(monitor_command_now==0x26000779) {
       printf("trd - трансляция десятичной цифры в шестнадцатиричную\n");
       }
     if(monitor_command_now==0x26000801) {
       printf("v - вывод рабочей информации общее\n");
       }
     if(monitor_command_now==0x26000802) {
       printf("vi  - вывод текущей иерархии (2017-05-30-04-17)\n");
       }
     if(monitor_command_now==0x26000803) {
       printf("vmi  - вывод текущего системного массива int\n");
       }
     if(monitor_command_now==0x26000804) {
       printf("vmt  - вывод текущего системного массива text\n");
       }
     if(monitor_command_now==0x26000805) {
       printf("va  - вывод первичного алгоритма\n");
       }
     if(monitor_command_now==0x26000806) {
       printf("va<nomer>  - вывод алгоритма <nomer>\n");
       }
     if(monitor_command_now==0x26000807) {
       printf("vсi  - вывод каталога иерархий\n");
       }
     if(monitor_command_now==0x26000808) {
       printf("vсv  - вывод каталога ветвей\n");
       }
     if(monitor_command_now==0x26000809) {
       printf("vi<nomer>  - вывод иерархии <nomer>\n");
       }
     if(monitor_command_now==0x26000810) {
       printf("vo<nomer> - вывод объекта <nomer>\n");
       }
     if(monitor_command_now==0x26000811) {
       printf("vni<name> - вывод иерархии с именем и её имя\n");
       }
     if(monitor_command_now==0x26000812) {
       printf("vno<name> - вывод объекта с именем и его имя\n");
       }
     if(monitor_command_now==0x26000813) {
       printf("izkh - загрузка текущей иерархии записи цифрами (2017-05-30-06-11)\n");
       }
     if(monitor_command_now==0x26000814) {
       printf("izkt - загрузка текущей иерархии записи банальным текстом (2017-05-30-06-22)\n");
       }
     if(monitor_command_now==0x26000821) {
       printf("i - загрузка рабочей информации общее\n");
       }
     if(monitor_command_now==0x26000822) {
       printf("imkd - загрузка текущего int массива с клавиатуры десятичное\n");
       }
     if(monitor_command_now==0x26000823) {
       printf("imkh - загрузка текущего int массива с клавиатуры шестнадцатиричное\n");
       }
     if(monitor_command_now==0x26000824) {
       printf("imf<file> - загрузка текущего int массива из файла\n");
       }
     if(monitor_command_now==0x26000825) {
       printf("imi<nomer> - загрузка текущего int массива из иерархии <nomer>\n");
       }
     if(monitor_command_now==0x26000826) {
       printf("imv<nomer> - загрузка текущего int массива из ветви <nomer>\n");
       }
     if(monitor_command_now==0x26000827) {
       printf("iikd - загрузка текущего int массива с клавиатуры десятичное\n");
       }
     if(monitor_command_now==0x26000828) {
       printf("iikh - загрузка текущего int массива с клавиатуры шестнадцатиричное\n");
       }
     if(monitor_command_now==0x26000829) {
       printf("iif<file> - загрузка текущего int массива из файла\n");
       }
     if(monitor_command_now==0x26000830) {
       printf("iii<nomer> - загрузка текущего int массива из иерархии <nomer>\n");
       }
     if(monitor_command_now==0x26000841) {
       printf("o - выгрузка рабочей информации общее\n");
       }
     if(monitor_command_now==0x26000842) {
       printf("omf<file> - выгрузка текущего int массива в файл\n");
       }
     if(monitor_command_now==0x26000843) {
       printf("omi<nomer> - выгрузка текущего int массива в иерархию <nomer>\n");
       }
     if(monitor_command_now==0x26000844) {
       printf("omv<nomer> - выгрузка текущего int массива в ветвь <nomer>\n");
       }
     if(monitor_command_now==0x26000845) {
       printf("otf<file> - выгрузка текущего text массива в файл\n");
       }
     if(monitor_command_now==0x26000846) {
       printf("oti - выгрузка текущего text массива в иерархию\n");
       }

//     if(massiv_int_keyboard[1]=='h') {
//       system_rejim1[0]=UES_rejim_prostoq;
//       monitor_help_info(massiv_system_text_buffer);
//       preobrazovanie_massiva_char_v_massiv_int(massiv_system_text_buffer, massiv_system_int_buffer);
//       vyvod_massiva_int(massiv_system_int_buffer);
//       }
//====
     send(sock2, system_rejim1, 4, 0);
     }
   sleep(5);
   printf("system_rejim1_2=%i\n",system_rejim1[0]);
   if(system_rejim1[0]==UES_rejim_hard_exit)
     goto address001;
   }
address001:
   printf("system_rejim1_2k1=%i\n",system_rejim1[0]);
 close(sock2);
   printf("system_rejim1_2k2=%i\n",system_rejim1[0]);

//=====
 unsigned int qwertyu1[10]={5,30,31,30,32};
 unsigned int qwertyu2[10];
 unsigned int qwertyu3[10];
 copy_unformat_text_to_UES_text(qwertyu2,qwertyu1,0x28000151);
 copy_UES_text_to_unformat_text(qwertyu3,qwertyu2);

 init_iera(0x1000100,7,8,9);
 copy_from_UES_text_to_iera(0x1000100,101, qwertyu2);
 for(i1=0;i1<25;i1++)
   printf("%i = %i\n",i1,massiv_iera[0x100][i1]);
 
 printf("Счастливо вам\n");
 return 0;
 }
// ==100== основная программа Универсальной Экспертной Системы
