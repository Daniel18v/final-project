<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header") ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>

<?php $this->stop() ?>

<?php $this->start('section') ?>
<div class="container-fluid mycontent">
    <div class="hr-margin-bottom-10"></div>
    <div class="row">
        <div class="col-lg-9 hidden-sm hidden-xs">
            <div class="panel panel-info">
                <div class="panel-heading">
                    <h3 class="panel-title">Panel title</h3>
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
                                <img src="http://cdn.akamai.steamstatic.com/steam/apps/489830/capsule_616x353.jpg?t=1481040018"
                                     alt="Image">
                                <div class="carousel-caption">
                                    <h3>Prueba 1</h3>
                                    <p>Lolololo</p>
                                </div>
                            </div>

                            <div class="item">
                                <img src="http://cdn.akamai.steamstatic.com/steam/apps/252490/capsule_616x353.jpg?t=1492098639"
                                     alt="Image">
                                <div class="carousel-caption">
                                    <h3>Prueba 2</h3>
                                    <p>Lorem ipsum...</p>
                                </div>
                            </div>
                        </div>
                        <div class="hr-margin-bottom-10"></div>
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
        </div>

        <div class="col-lg-3 ">
            <div class="panel panel-info">
                <div class="panel-heading">
                    <h3 class="panel-title">Quiénes somos</h3>
                </div>
                <div class="panel-body">
                    <div class="panel panel-info">
                        <div class="panel-heading">
                            <h3 class="panel-title">Yo</h3>
                        </div>
                        <div class="panel-body">
                           Aquí van cosas
                        </div>
                    </div>
                    <div class="panel panel-info">
                        <div class="panel-heading">
                            <h3 class="panel-title">Tú</h3>
                        </div>
                        <div class="panel-body">
                            Aquí van cosas
                        </div>
                    </div>
                </div>
            </div>


            <div class="panel panel-info">
                <div class="panel-heading">
                    <h3 class="panel-title">Afiliados</h3>
                </div>
                <div class="panel-body">
                    <a href="http://animeomega.es"><img src="http://i.imgbox.com/nxJzSEbl.jpg" class="img-responsive center-block"></a>
                </div>
            </div>

            <div class="panel panel-info">
                <div class="panel-heading">
                    <h3 class="panel-title">Chat</h3>
                </div>
                <div class="panel-body">
                    <?= (!isset($_SESSION['nombre'])) ? $this->insert("chat::chat") : "" ?>
                </div>
            </div>
        </div>
    </div>
    <div class="hr-margin-bottom-10"></div>




    <div class="row">
        <div class="col-lg-12">
            <?= $this->insert("structure::signin") ?>
            <?= $this->insert("structure::login") ?>
        </div>
    </div>

</div>

<?php $this->stop() ?>
