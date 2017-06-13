var bg;
var tubos;
var flappy;
var salto;
var timer;
var puntos;
var txtPuntos;

var Juego = {

    preload: function(){
        juego.load.image('bg', '/apps/flappybird/img/bg.jpeg');
        juego.load.spritesheet('pajaros', '/apps/flappybird/img/pajaro.png', 43, 30);
        juego.load.image('tubo', '/apps/flappybird/img/tubo.png');

        juego.forceSingleUpdate = true;
    },

    create: function(){
        bg = juego.add.tileSprite(0, 0, 370, 550, 'bg');
        juego.physics.startSystem(Phaser.Physics.ARCADE);
        tubos = juego.add.group();
        tubos.enableBody = true;
        tubos.createMultiple(20, 'tubo');

        flappy = juego.add.sprite(100, 245, 'pajaros');
        flappy.frame = 1;
        flappy.anchor.setTo(0, 0.5);
        flappy.animations.add('vuelo', [0,1,2], 10, true);

        juego.physics.arcade.enable(flappy);
        flappy.body.gravity.y = 1200;

        salto = juego.input.keyboard.addKey(Phaser.Keyboard.SPACEBAR);
        salto.onDown.add(this.saltar, this);

        timer = juego.time.events.loop(1500, this.crearColumna, this);

        puntos = -1;
        txtPuntos = juego.add.text(20, 20, "0", {font: "30px Arial", fill: "#FFF"});

    },

    update: function(){
        if(flappy.inWorld == false)
        {
            //Reiniciar = enviar a Game_Over
            this.addCoins();
            this.state.start('Game_Over');
        }
        else if(flappy.position.y >460)
        {
            //Reiniciar = enviar a Game_Over
            //alert(puntos)
            flappy.alive = false;
            tubos.forEachAlive(function(t){
                   t.body.velocity.x = 0;
            }, this);
        } else {
            bg.tilePosition.x -= 1;
        }

        juego.physics.arcade.overlap(flappy, tubos, this.tocoTubo, null, this);

        flappy.animations.play('vuelo');
        if(flappy.angle <20)
        {
            flappy.angle += 1;
        }
    },

    saltar: function(){
        if(flappy.alive === true) {
            flappy.body.velocity.y = -350;
            juego.add.tween(flappy).to({angle:-20}, 100).start();
        }

    },

    crearColumna: function(){
        var hueco = Math.floor(Math.random()*5)+1;
        for( var i = 0; i < 8; i++)
        {
            if(i != hueco && i != hueco+1)
            {
                this.crearUnTubo(370, i*55+20);
            }
        }

        puntos +=1;
        txtPuntos.text = puntos;
    },

    crearUnTubo: function(x, y){
        var tubo = tubos.getFirstDead();

        tubo.reset(x, y);
        tubo.body.velocity.x = -180;
        tubo.checkWorldBounds = true;
        tubo.outOfBoundsKill = true;
    },

    tocoTubo: function(){
        if(flappy.alive == false)
            return;
        flappy.alive = false;
        juego.time.events.remove(timer);


        tubos.forEachAlive(function(t){
            t.body.velocity.x = 0;
        }, this);
    },

    addCoins: function(){
        if(puntos < 0) {
            puntos = 0;
        }
        $.ajax({
            url: '/add_coins',
            data: {"bird_coins": puntos},
            type: 'POST',
            dataType: 'json',
            success: function (coins) {
                $('#coins a span:first-child ').text(coins);
            }
        })

    }

};
