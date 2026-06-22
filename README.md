# Модель сегрегации Шел­линга

Для компиляции нужно установить библиотеки:

```sh
# Fedora (dnf)
sudo dnf groupinstall "Development Tools"
sudo dnf install fltk fltk-devel fltk-fluid
```

```sh
# Ubuntu/Debian (apt)
sudo apt update
sudo apt install libfltk1.3-dev fluid build-essential
```

И потом скомпилировать через команду: `make`

Сборка и запуск через Docker:
```sh
git clone https://github.com/simadude/schelling-segregation-model.git
cd schelling-segregation-model
docker build -t schelling-segregation-model .
xhost +local:docker
docker run --net=host -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix schelling-segregation-model
```

Сборка без Docker:

1. Скачать репозиторий.
2. Установить нужные библиотеки (см. выше).
3. Скомпилировать через `make`.
4. Запустить `./build/bin/schelling`.

Для графического интерфейса данный код использует FLTK (http://www.fltk.org)