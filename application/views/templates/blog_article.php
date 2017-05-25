<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>


<?php $this->layout('blog', ['title' => $article->title]) ?>

<h2><?=$this->e($article->title)?></h2>
<article>
    <?=$this->e($article->content)?>
</article>