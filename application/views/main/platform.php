<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="container-fluid mycontent">
    <div class="hr-margin-bottom-10"></div>
    <div class="row">
        <?php foreach ($roms as $rom => $values) : ?>
        <div class="col-lg-3">
            <div class="panel panel-info">

                <div class="panel-heading">
                    <h3 class="panel-title"><?= $values['title'] ?></h3>
                </div>
                <div class="panel-body">
                    <div class="row text-center">
                        <div class="col-lg-12">
                            <div class="thumbnail">
                                <img src="<?= $values['image'] ?>" alt="<?= $values['title'] ?>" class="img-responsive">
                                <div class="caption">
                                    <h3><?= $values['title'] ?></h3>
                                    <p class="min-height-90"><?= substr($values['description'], 0, 160) ?></p>
                                    <p><a href="<?= $values['type'] . "/" . $values['slug'] ?>" class="btn btn-primary" role="button">Jugar</a>
                                    </p>
                                </div>
                            </div>

                        </div>
                    </div>
                </div>
            </div>
        </div>
        <?php endforeach ?>
    </div>
</div>

<?php $this->stop() ?>
