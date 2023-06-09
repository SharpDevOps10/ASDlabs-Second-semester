# ASDlabs-Second-semester
These are labs from the second semester 
## Лабораторна робота № 2.1
Дане натуральне число n. Знайти суму перших n 
членів ряду чисел, заданого рекурентною формулою. Розв’язати задачу 
трьома способами (написати три програми): 

* 1) в програмі використати 
рекурсивну процедуру або функцію, яка виконує обчислення і членів ряду, і 
суми на рекурсивному спуску; 
* 2) в програмі використати рекурсивну 
процедуру або функцію, яка виконує обчислення і членів ряду, і суми на 
рекурсивному поверненні; 
* 3) в програмі використати рекурсивну процедуру 
або функцію, яка виконує обчислення членів ряду на рекурсивному спуску, а 
обчислення суми на рекурсивному поверненні. Програми повинні працювати 
коректно для довільного натурального n включно з n = 1.


![изображение](https://user-images.githubusercontent.com/71943754/224564603-261f3976-079f-4c65-9a7f-14674e0006f2.png)
## Лабораторна робота № 2.2

1. Створити список з n (n>0) елементів (n вводиться з клавіатури), якщо інша кількість елементів не вказана у конкретному завданні.
2. Тип ключів (інформаційних полів) задано за варіантом. 
3. Значення елементів списку взяти самостійно такими, щоб можна було продемонструвати коректність роботи алгоритму програми. Введення значень елементів списку можна виконати довільним способом (випадкові числа, формування значень за формулою, введення з файлу чи з клавіатури).  
4. Вид списку (черга, стек, дек, прямий однозв’язний лінійний список, обернений однозв’язний лінійний список, двозв’язний лінійний список, однозв’язний кільцевий список, двозв’язний кільцевий список) вибрати самостійно з метою найбільш доцільного рішення поставленої за варіантом задачі. 
5. Виконати над створеним списком дії, вказані за варіантом, та коректне звільнення пам’яті списку. 
6. При виконанні заданих дій, виводі значень елементів та звільненні пам’яті списку вважати, що довжина списку (кількість елементів n чи 2n) невідома на момент виконання цих дій. 
7. Повторювані частини алгоритму необхідно оформити у вигляді процедур або функцій (для створення, обробки, виведення та звільнення пам’яті списків) з передачею списку за допомогою параметра(ів)

Варіант 20 

Ключами елементів списку є цілі числа. Кількість елементів списку повинна дорівнювати 2n. Перекомпонувати елементи списку так, розташування елементів було наступним: a1, an + 1, a2, an + 2, a3, …, an, a2n, де ai − i-й компонент списку, не використовуючи додаткових структур даних, крім простих змінних (тобто «на тому ж місці»).

## Лабораторна робота № 2.3
Представити у програмі напрямлений і ненапрямлений графи з
заданими параметрами:

— число вершин n;

— розміщення вершин;

— матриця суміжності A.

Параметри задаються на основі номера групи, представленого десятковими цифрами п1, п2 та номера студента у списку групи — десяткового числа n3, n4.

Число вершин n дорівнює 10 + n3.

Розміщення вершин:

— колом при n4 = 0,1;

— прямокутником (квадратом) при n4 = 2,3;

— трикутником при n4 = 4,5;

— колом з вершиною в центрі при n4 = 6,7;

— прямокутником (квадратом) з вершиною в центрі при n4 = 8,9.

Наприклад, при п4 = 10 розміщення вершин прямокутником з 
вершиною в центрі повинно виглядати так, як на прикладі графа рис.4.

Матриця А напрямленого графа за варіантом формується за функціями:

* srand(n1 n2 n3 n4);

* T = randm(n,n);

* A = mulmr((1.0 - n3*0.02 - n4*0.005 - 0.25),T);

де randm(n,n) – розроблена функція, яка формує матрицю розміром n*n,
що складається з випадкових чисел у діапазоні (0, 2.0);

mulmr() — розроблена функція множення матриці на коефіцієнт та
округлення результату до 0 чи 1 (0, якщо результат менший за 1.0 і 1 — якщо
більший за 1.0).

2. Створити програму для формування зображення напрямленого і
ненапрямленого графів у графічному вікні.

`Варіант : 2220.`

Число вершин n = 10 + 2 = 12.

Розміщення вершин : колом, бо n4 = 0.

Srand (2220);

## Лабораторна робота № 2.4

Постановка задачі : 1. Представити напрямлений граф з заданими 
параметрами так само, як у лабораторній роботі No3. 


Відміна: матриця А напрямленого графа за варіантом формується за 
функціями: 


srand(п1 п2 п3 п4); T = randm(n,n); A = mulmr(( 1.0 – n3*0.01 – n4*0.01 − 
0.3)*T); 


Перетворити граф у ненапрямлений. 


2. Визначити степені вершин напрямленого і ненапрямленого графів. 
Програма на екран виводить степені усіх вершин ненапрямленого графу і 
напівстепені виходу та заходу напрямленого графу. Визначити, чи граф є 
однорідним та якщо так, то вказати степінь однорідності графу. 


3. Визначити всі висячі та ізольовані вершини. Програма на екран виводить 
перелік усіх висячих та ізольованих вершин графу. 


4. Змінити матрицю графу за функцією A = mulmr(( 1.0 – n3*0.005 – n4*0.005 
− 0.27)*T); 


Створити програму для обчислення наступних результатів: 
1) матриця суміжності;
2) півстепені вузлів; 
3) всі шляхи довжини 2 і 3; 
4) матриця досяжності; 
5) компоненти сильної зв’язності; 
6) матриця зв’язності; 
7) граф конденсації. Шляхи довжиною 2 і 3 слід шукати за матрицями А2 і 
А3 , відповідно. Матриця досяжності та компоненти сильної зв’язності слід 
шукати за допомогою операції транзитивного замикання.

`Завдання варіанту № 20 : `


* Номер – 2220
* Число вершин = 10 +2 = 12.
* Розміщення вершин колом , бо n4 = 0;
* Srand(2220)

## Лабораторна робота № 2.5

Постановка задачі : 

* 1. Представити напрямлений граф з заданими параметрами так само, як у
лабораторній роботі No3. Відміна: матриця А за варіантом формується за
функцією:

`A = mulmr(( 1.0 – n3*0.01 – n4*0.005 − 0.15)*T);`
* 2. Створити програми для обходу в глибину та в ширину. Обхід
починати з вершини, яка має вихідні дуги. При цьому у програмі:
— встановити зупинку у точці призначення номеру черговій вершині за
допомогою повідомлення про натискання кнопки,
— виводити зображення графа у графічному вікні перед кожною
зупинкою.

* 3. Під час обходу графа побудувати дерево обходу. Вивести побудоване
дерево у графічному вікні.

`Завдання варіанту № 20 :` 

* Номер – 2220
* Число вершин = 10 +2 = 12.
* Розміщення вершин колом , бо n4 = 0;
* Srand(2220)
