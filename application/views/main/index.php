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
    <div class="col-lg-2">

    </div>
    <div class="col-lg-5">
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
        <!-- POST BLOG -->
        <div class="panel panel-info posted-blog">
            <div class="pull-left media">

            </div>
            <div class="media-body">
                <img id="img-prev" src="/images/roms/nes/Chip_'n_Dale_Rescue_Rangers(3).png" class="img-responsive">
                <h4 id="title-prev" class="media-heading text-center">Nuevo juego en nuestra sección NES</h4>
                <p id="description-prev">Lorem ipsum dolor sit amet, consectetur adipisicing elit. Architecto,
                    consectetur cupiditate ducimus hic iste maiores, natus nisi nobis numquam omnis pariatur quas qui
                    tempore veniam voluptatibus? Asperiores id nostrum veritatis?</p>

                <ul class="list-inline list-unstyled">
                    <li><span><i class="glyphicon glyphicon-calendar"></i> 2 days, 8 hours </span></li>
                    <li>|</li>
                    <span><i class="glyphicon glyphicon-comment"></i> 2 comments</span>
                    <li>|</li>
                </ul>
            </div>
        </div>
    </div>
    <div class="col-lg-3">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h3 class="panel-title">Quiénes somos</h3>
            </div>
            <div class="panel-body">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="panel-title">Trabajo de final de curso</h3>
                    </div>
                    <div class="panel-body">
                        <p>Esta página es un trabajo de final de curso.<br/>Aún se encuentra en construcción.</p>
                    </div>
                </div>
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
                <h3 class="panel-title">Chat</h3>
            </div>
            <div class="panel-body">
                <?= !$sess->session->userdata('nombre') ? $this->insert("chat::chat", ['sess' => $sess]) : "" ?>
            </div>
        </div>
    </div>
    <div class="col-lg-2 col-xs-0">

    </div>
</div>
<div>
    <?= $this->insert("structure::models", ['sess' => $sess]) ?>
</div>
<?php $this->stop() ?>
