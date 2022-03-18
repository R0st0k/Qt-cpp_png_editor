## ANNOTATION

In this work, a program was created in the C++ programming language using the Qt5 framework. The program reads, processes and writes PNG files. When the program starts, only a few buttons are active, including a button to open a file. 

After opening the file, image processing actions are available:

- Drawing a circle by selecting two points on the image - the ends of the diameter. Additionally, it is proposed to select the line thickness, its color, the presence and color of the fill.
- Changing an RGB component by choosing from three components and scaling the new value for that component
- Dividing into parts by choosing the number of parts, thickness and color of the division line
- Drawing a square by selecting two points on the image - one of the vertices and the length of the edge of the square. Additionally, it is proposed to select the line thickness, its color, the presence and color of the fill.
- Stroke all white areas of the image with the selected color, 1 pixel in size

The program will continue to run until it is closed. 

## Run instructions

To run the program, Qt, libpng and zlib libraries are required:

$ sudo apt-install qt5-default

$ sudo apt-get install libpng-dev

$ sudo apt-get install zlib1g-dev

Being in the directory with the program, perform the following actions:

1) $ qmake (Automatically generates a Makefile)
2) $ make (PNG_editor executable is created)
3) $ ./PNG_editor (The program starts) 

## Launch example

You can see the program launch record below:

https://drive.google.com/file/d/1JMaotVdvSGVCkVUA_XSKnARpUGG4HDgd/view?usp=sharing

Processed drawings can be found in the folder Examples

P.s. Outlining the white areas didn't work because the green component of each pixel was set to "10" beforehand. Therefore, there could be no white areas 

---

## АННОТАЦИЯ

В данной работе была создана программа на языке программирования С++, используя фреймворк Qt5. Программа производит чтение, обработку и запись PNG файлов. При запуске программы только несколько кнопок являются активными, в том числе кнопка для открытия файла. 

После открытия файла доступны действия по обработке изображения:

- Рисование окружности путем выбора на изображении двух точек — концов диаметра. Дополнительно предлагается выбрать толщину линии, её цвет, наличие и цвет заливки
- Изменение RGB-компонента путем выбора из трех компонентов и величиной нового значения для данного компонента
- Деление на части путем выбора количества частей, толщины и цвета линии разделения
- Рисование квадрата путем выбора на изображении двух точек — одной из вершин и длины ребра квадрата. Дополнительно предлагается выбрать толщину линии, её цвет, наличие и цвет заливки
- Обводка всех белых областей картинки выбранным цветом размером в 1 пиксель

Программа продолжит выполнение до тех пор, пока не будет закрыта.

## Инструкция по запуску

Для запуска программы требуется Qt, библиотеки libpng и zlib:

$ sudo apt-install qt5-default

$ sudo apt-get install libpng-dev

$ sudo apt-get install zlib1g-dev

Находясь в директории с программой, выполняем следующие действия:

1) $ qmake        (Автоматически генерирует Makefile)
2) $ make         (Создается исполняемый файл PNG_editor)
3) $ ./PNG_editor (Запускается программа)

## Пример запуска

Запись запуска программы можно увидеть ниже:

https://drive.google.com/file/d/1JMaotVdvSGVCkVUA_XSKnARpUGG4HDgd/view?usp=sharing

Обрабатываемые рисунки можно найти в папке Examples

P.s. Обводка белых областей не показала результата, потому что предварительно зеленый компонент каждого пикселя был приравнен к "10". Следовательно, белых областей быть не могло
