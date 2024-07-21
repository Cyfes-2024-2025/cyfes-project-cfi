# Writing an LLVM Optimization

Comincia spiegando perché uno dovrebbe voler scrivere una ottimizzazione.
L'esempio che fa è di dire: immagina di voler confrontare due numeri fp con ==. Il risultato non sarà preciso a causa
di rounding errors. Quello che faresti allora è calcolarti la differenza in valore assoluto tra i due numeir
e confrontare il valore che ti viene rispetto ad un parametro fissato epsilon. Se ci stai dentro allora i due
numeri sono quasi uguali.

Anziché scriverti il codice a mano potresti scrivere una LLVM optimization che lo faccia al posto tuo.
(Ovviamente è una brutissima idea perché cambi la semantica del linguaggio ma è solo per fare un esempio).

## Useful flags for compilation

```
-DLLVM_ENABLE_PROJECTS="cland;lld"
// serve per non installare llvm nel path dell'installazione "originale" di macos
-DCMAKE_INSTALL_PREFIX=/home/user/custom-llvm
// and then to not write the entire thing everytime
export PATH=$PATH:/home/user/custom-llvm/bin
// per debuggare la pass
-DCMAKE_BUILD_TYPE="Debug"
// tbh non ho capito a cosa serva, dice che normalmente non ti serve a meno che tu non faccia "table related development"
-DLLVM_OPTIMIZED_TABLEGEN=TRUE
```

Esempio di flow

```shell
mkdir my-custom-passes
cd my-custom-passes
git clone llvm-project
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE="Debug" \
  -DCMAKE_C_COMPILER=/usr/bin/gcc \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
  -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja \
  -DCMAKE_INSTALL_PREFIX=/homoe/user/custom-llvm \
  -DLLVM_ENABLE_PROJECTS="clang;lld" \
  -DLLVM_OPTIMIZED_TABLEGEN=TRUE ../llvm-project/llvm
 cmake --build .
 cmake --build . --target install
```

## LLVM introduction

L'idea è sempre la stessa

1. Source code
2. Intermediate Representation IR (code è target independent ma non platform independent)
3. Machine Intermediate Representation Machine IR (qua il code comincia ad assomigliare ad assembly)
4. Assembly (ci arriviamo grazie a LLVM target backend)
5. Object code
6. Linker fa final linkage/executable

### LLVM IR

Si parla di **moduli**: un modulo lo puoi vedere come un file C/C++ compilato.
È composto da una sezione con le variabili globali, da una sezione con le funzioni (a loro volta composte da
**basic block** che sono composti da **instructions**), da una sezione contenente le dipendenze (o librerie o altri moduli)
e dalla **symbol table**.

Noi siamo interessanti a modificare le istructions nei basic blocks ma in realtà potremmo lavorare su ogni componente del modulo
(incluso il modulo stesso) .

### Source del materiale

[Writing an LLVM Optimization](https://www.youtube.com/watch?v=MagR2KY8MQI&t=1332s)

--target=arm64e -O0 -mbranch-protection=pac-ret -S -emit-llvm

Cosa ho fatto:
Andato dentro cyfes-project-cfi/llvm-project-ptrauth/build
Lanciato
cmake -G Ninja -DCMAKE_BUILD_TYPE="Debug" \
-DCMAKE_INSTALL_PREFIX=/tmp/test_build \
-DLLVM_ENABLE_PROJECTS="clang;lld" \
-DLLVM_OPTIMIZED_TABLEGEN=TRUE ../llvm
cmake --build .
cmake --build . --target installx
