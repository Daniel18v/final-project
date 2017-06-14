<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="row">
    <div class="col-lg-1">

    </div>
    <div class="col-lg-7">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h3 class="panel-title">Juegos destacados</h3>
            </div>
            <div class="panel-body">
                <div id="myCarousel" class="carousel slide" data-ride="carousel">
                    <!-- Indicators -->
                    <ol class="carousel-indicators">
                        <li data-target="#myCarousel" data-slide-to="0" class="active"></li>
                        <li data-target="#myCarousel" data-slide-to="1" class=""></li>
                    </ol>

                    <!-- Wrapper for slides -->
                    <div class="carousel-inner" role="listbox">
                        <div class="item active">
                            <img src="http://www.journaldugamer.com/files/2014/07/flappy-bird-banner.jpg"
                                 alt="Image">
                            <div class="carousel-caption">
                                <h3>Flappy Bird</h3>
                                <p><a href="/juegos/flappybird" class="btn btn-info" role="button">Jugar</a></p>
                            </div>
                        </div>

                        <div class="item">
                            <img src="http://media.alvanista.com/uploads/game/34/47774/medium_4_screenshot.png"
                                 alt="Image">
                            <div class="carousel-caption">
                                <h3>Galaga</h3>
                                <p><a href="/juegos/galaga" class="btn btn-info" role="button">Jugar</a></p>
                            </div>
                        </div>
                    </div>
                    <!-- Left and right controls -->
                    <a class="left carousel-control" href="#myCarousel" role="button" data-slide="prev">
                        <span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
                        <span class="sr-only">Previous</span>
                    </a>
                    <a class="right carousel-control" href="#myCarousel" role="button" data-slide="next">
                        <span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
                        <span class="sr-only">Next</span>
                    </a>
                </div>
            </div>
        </div>
        <div class="panel panel-info adjust">
            <div class="panel-heading">
                <h3 class="panel-title ">Noticias</h3>
            </div>
        </div>
        <!-- POST BLOG -->
        <?php foreach ($blogs as $post => $values) : ?>
            <div class="panel panel-info posted-blog">
                <div class="pull-left media">

                </div>
                <div class="media-body">
                    <img id="img-prev" src="<?= $values['image'] ?>" class="img-responsive">
                    <a href="<?= $values['slug'] ?>"><h4 id="title-prev"
                                                         class="media-heading text-center"><?= $values['title'] ?></h4>
                    </a>
                    <p id="description-prev"><?= $values['description'] ?></p>

                    <ul class="list-inline list-unstyled">
                        <li><span><i class="glyphicon glyphicon-calendar"></i> <?= $values['timestamp'] ?> </span></li>
                        <li>|</li>
                        <span><i class="glyphicon glyphicon-comment"></i> <?= $values['total_comments'] ?>
                            comments</span>
                        <li>|</li>
                    </ul>
                </div>
            </div>
        <?php endforeach; ?>

    </div>
    <div class="col-lg-3">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h3 class="panel-title">Sidebar</h3>
            </div>
            <div class="panel-body">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Chat</h3>
                    </div>
                    <div class="panel-body">
                        <?= ! $sess->session->userdata('nombre') ? $this->insert("chat::chat", ['sess' => $sess]) : "" ?>
                    </div>
                </div>

                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Afiliados</h3>
                    </div>
                    <div class="panel-body">
                        <a href="http://animeomega.es"><img src="http://i.imgbox.com/nxJzSEbl.jpg"
                                                            class="img-responsive center-block"></a>
                    </div>
                </div>

                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Publicidad</h3>
                    </div>
                    <div class="panel-body">
                        <a href="https://www.game.es/nintendo-classic-mini-nes-electronica-129248"><img src="https://i.kinja-img.com/gawker-media/image/upload/s--QUnZozJ9--/c_scale,f_auto,fl_progressive,q_80,w_800/bpmpwr43w8r0hvzbjvgb.gif"
                                                            class="img-responsive center-block"></a>
                    </div>
                </div>
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Trabajo de final de curso</h3>
                    </div>
                    <div class="panel-body">
                        <img class="img-responsive"
                             src="http://k45.kn3.net/taringa/1/4/0/4/9/1/25/3dm0nd84/3CF.gif?7340">
                        <div class="hr-margin-top-10">
                            <ul>
                                <li>Alumnos:</li>
                                <ul>
                                    <li>Germán Boquizo Sánchez</li>
                                    <li>Daniel Rodríguez Cabrera</li>
                                    <li>Jose Luis Hernández Asenjo</li>
                                </ul>
                                <li class="hr-margin-top-10">Curso: 2.º de DAW</li>
                            </ul>
                        </div>
                        <p class="text-justify">Este trabajo ha sido desarrollado para el proyecto de fin de ciclo. Hemos creado una
                            plataforma de videojuegos en la que se pueden jugar a juegos, tanto de HTML5, como de
                            diversos emuladores.
                        </p>
                        <p>
                            Si bien es cierto, que nos han quedado cosas en desarrollo, y cosas que nos hubieran gustado
                            implementar. La falta de tiempo ha sido crucial en el desempeño de este trabajo.</p>
                        <img class="img-responsive" src="http://k32.kn3.net/taringa/1/4/0/4/9/1/25/3dm0nd84/ABE.gif?7748">
                    </div>
                </div>



            </div>
            <div class="col-lg-1 col-xs-0">

            </div>
        </div>
    </div>
</div>
<?php $this->stop() ?>
