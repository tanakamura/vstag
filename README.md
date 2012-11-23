tagの付けかたについて考える

 - ruby方式 : ポインタにタグが入ってる
 - lua方式  : ポインタを保持するコンテナにタグが入ってる

<code>
 $ cmake . -DCMAKE_BUILD_TYPE=Release
 $ make
 $ time ./vstag -t fixnum -T ruby # ruby 方式でfixnum
 $ time ./vstag -t fixnum -T lua  # lua 方式でfixnum
 $ time ./vstag -t float -T ruby  # ruby 方式で倍精度
 $ time ./vstag -t float -T lua   # lua 方式で倍精度
</code>


gc.[ch]pp は… 何をやろうとしたんだっけ覚えてない。