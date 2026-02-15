gcc TestEntity.c ../Source/entity.c ./Unity/unity.c -o TestEntity.exe
rem gcc TestConfigure.c ../Source/configure.c ../Source/world.c ../Source/region.c ../Source/entity.c ../Source/mapping.c ./Unity/unity.c -o TestConfigure.exe
gcc TestMapping.c ../Source/mapping.c ./Unity/unity.c -o TestMapping.exe
gcc TestRegion.c ../Source/Region.c ./Unity/unity.c ../Source/mapping.c ../Source/entity.c -o TestRegion.exe