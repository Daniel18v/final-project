<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="container">
    <div class="row">
        <div class="col-lg-12">

            <!-- POST BLOG -->
            <?php foreach ($blogs as $post => $values) : ?>
                <div class="panel panel-info posted-blog">
                    <div class="pull-left media">

                    </div>
                    <div class="media-body">
                        <img id="img-prev" src="<?= $values['image'] ?>" class="img-responsive">
                        <a href="<?= $values['slug'] ?>"><h4 id="title-prev" class="media-heading text-center"><?= $values['title'] ?></h4></a>
                        <p id="description-prev"><?= $values['description'] ?></p>

                        <ul class="list-inline list-unstyled">
                            <li><span><i class="glyphicon glyphicon-calendar"></i> <?= $values['timestamp'] ?> </span></li>
                            <li>|</li>
                            <span><i class="glyphicon glyphicon-comment"></i> <?= $values['total_comments'] ?> comments</span>
                            <li>|</li>
                        </ul>
                    </div>
                </div>
            <?php endforeach; ?>

        </div>

    </div>
</div>
<?php $this->stop() ?>
