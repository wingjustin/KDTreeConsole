# KDTreeConsole

<h3>KD tree</h3>

<p>

KD樹 為二叉樹
<br><br>
<h4>建樹</h4>
<br>
取得座標組,先計算各維度的方差, 選擇方差最大維度為切割維度。
<br>
然後取得切割維度上的中位數為切割點,這裡用快速排序不完全排序獲得中位數,只遞迴排序包括中位數所在位置的一側,這樣遞迴可以改成迭代。
<br><br>

<h4>BBF算法</h4>
先從根找到葉節點,過程中找最近鄰點,並把對面節點按分割的超平面與查詢點的距離進行排序壓入優先隊列, 等到葉節點再把優先隊列取得最近節點再找一次有沒有其他最近鄰點


<br><br>
<div>References : </div>
<ul>
<li>Moore, Andrew W. "An intoductory tutorial on kd-trees." (1991).  https://www.ri.cmu.edu/pub_files/pub1/moore_andrew_1991_1/moore_andrew_1991_1.pdf</li>
<li>https://zhuanlan.zhihu.com/p/72725372</li>
<li>https://blog.csdn.net/lhanchao/article/details/52535694</li>
<li>https://cloud.tencent.com/developer/beta/article/2095315</li>
<li>https://oi-wiki.org/ds/kdt/</li>
</ul>
</p>
