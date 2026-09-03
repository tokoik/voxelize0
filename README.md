# voxelize0

## 1. 概要

このプログラムは、OpenGL のフレームバッファオブジェクト (FBO) と平行投影のクリッピングプレーンを利用して、3次元ポリゴンモデル（ティーポット等）の断面形状を順次スライス取得し、ボクセルデータ（3次元ボリューム配列）へ変換（ボクセル化: solid voxelization）する手法を学ぶためのサンプルプログラムです。本プログラムは、以下のブログ記事の解説に対応しています。

- [とっても簡単なボクセル化](https://tokoik.github.io/blog/2009-10-07.html)

クリッピングされた前方面の断面形状を論理演算反転 (`GL_INVERT`) を用いて抽出し、FBO へレンダリングした結果を `glReadPixels()` で CPU 側のメモリに読み出します。実行後は生成したボクセルデータを 5 秒周期で順次断面スライス表示します。

## 2. 対応環境

- **Windows**: Visual Studio 2019 以降 / CMake 3.22 以降
- **macOS**: Xcode 12 以降 / CMake 3.22 以降
- **Linux (Ubuntu 等)**: GCC / Clang / CMake 3.22 以降

## 3. ビルド手順

### 3.1 Windows (Visual Studio)

```powershell
cmake -B build
cmake --build build --config Release
```

### 3.2 Linux (Ubuntu)

必要なパッケージをインストールした上でビルドします。

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev libglew-dev
cmake -B build
cmake --build build
```

### 3.3 macOS (Xcode)

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

## 4. 起動方法

ビルド完了後、生成された実行ファイルを実行します。

```powershell
./build/Release/voxelize0.exe
```

## 5. 操作方法

- 起動すると、初期化時に 256x256x256 のボクセル化処理が行われます。
- ボクセル化完了後、時間経過に伴って Z 軸方向の断面スライス（256x256 ピクセル）が順次アニメーション表示されます。
- ウィンドウを閉じるかコンソールで `Ctrl+C` を押すと終了します。

## 6. プログラムの解説

### 6.1 クリッピングによる断面の抽出

境界箱の前方面の位置に視点を設定し、前方面の位置（`offset`）を奥へずらしながら `glOrtho()` で投影変換を設定します。

```cpp
/* 境界箱を視野空間に設定する */
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(pmin[0], pmax[0], pmin[1], pmax[1], offset, pmax[2] - pmin[2]);
glMatrixMode(GL_MODELVIEW);
```

### 6.2 フレームバッファ反転 (`GL_INVERT`)

ポリゴンが偶数回描画される（中身のない領域）部分は元の背景色に戻り、奇数回描画される（断面の内部）部分のみが反転色で残るため、断面形状の塗りつぶしが得られます。

```cpp
/* フレームバッファに書きこむたびにフレームバッファの内容を反転する */
glLogicOp(GL_INVERT);

/* 隠面消去処理は行わない */
glDisable(GL_DEPTH_TEST);
```

### 6.3 FBO からのデータ読み出し

各スライスの描画結果を `glReadPixels()` を用いてボクセル配列へ格納します。

```cpp
/* フレームバッファオブジェクトからデータを読み出す */
glReadPixels(0, 0, sx, sy, GL_RED, GL_UNSIGNED_BYTE, v + sx * sy * z);
```
