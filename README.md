# Библиотека аллокаторов игры TGOX

Включает в себя раеализацию линейного, стекового и пулового аллокаторов.

# Сборка

Мейкфаил настроен под clang с MinGW - для сборки с clang и gcc уберите флаг target.

git clone https://github.com/xiran56/tgAllocator.git

cd tgAllocator

make init

make

В директории lib будет лежать фаил libtgallocator.a - собранная библиотека.

Переход на CMake в ближайшее время не планируется.
