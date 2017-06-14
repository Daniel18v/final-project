<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
    <div class="container">
        <div class="row">
            <div class="col-xs-12 col-sm-12 col-md-6 col-lg-6 col-xs-offset-0 col-sm-offset-0 col-md-offset-3 col-lg-offset-3 toppad" >
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Listado de roms compradas</h3>
                    </div>
                    <div class="panel-body">
                        <?php foreach($roms as $key => $value): ?>
                        <div class="row">
                            <div class="col-md-3 col-lg-3"> <img alt="User Pic" src="<?= explode(',', $value['image'])[0] ?>" class="img-circle img-responsive"> </div>
                            <div class=" col-md-9 col-lg-9 ">
                                <table class="table table-user-information">
                                    <tbody>
                                    <tr>
                                        <td class="width-5">Título:</td>
                                        <td class="width-50"><h4><?= $value['title'] ?></h4></td>
                                    </tr>
                                    <tr>
                                        <td>Enlace</td>
                                        <td><a href="/juegos/emulador/<?= $value['type'] ?>/<?= $value['slug'] ?>"><?= $value['title'] ?></a></td>
                                    </tr>

                                    <tr>
                                        <td>Consola</td>
                                        <td><?= $value['type'] ?></td>
                                    </tr>
                                    </tbody>
                                </table>

                            </div>


                        </div>
                        <?php endforeach ?>
                    </div>


                </div>
            </div>
        </div>
    </div>
<?php $this->stop() ?>