//
// MoonWarriors
//
// Handles the Game Logic
//

STATE_PLAYING = 0;
STATE_GAMEOVER = 1;
MAX_CONTAINT_WIDTH = 40;
MAX_CONTAINT_HEIGHT = 40;

var g_sharedGameLayer;

var GameLayer = cc.Layer.extend({
    _time: null,
    _ship: null,
    _backSky: null,
    _backSkyHeight: 0,
    _backSkyRe: null,
    _levelManager: null,
    _tmpScore: 0,
    _isBackSkyReload: false,
    _isBackTileReload: false,
    lbScore: null,
    screenRect: null,
    explosionAnimation: [],
    _beginPos: cc.p(0, 0),
    _state: STATE_PLAYING,
    _explosions: null,
    _texOpaqueBatch: null,
    _texTransparentBatch: null,

    ctor: function () {
        this._super();
        this.init();
    },
    init: function () {
        cc.spriteFrameCache.addSpriteFrames(res.textureOpaquePack_plist);
        cc.spriteFrameCache.addSpriteFrames(res.b01_plist);

        // reset global values
        MW.CONTAINER.ENEMIES = [];
        MW.CONTAINER.ENEMY_BULLETS = [];
        MW.CONTAINER.PLAYER_BULLETS = [];
        MW.CONTAINER.EXPLOSIONS = [];
        MW.CONTAINER.SPARKS = [];
        MW.CONTAINER.HITS = [];
        MW.CONTAINER.BACKSKYS = [];
        MW.CONTAINER.BACKTILEMAPS = [];
        MW.ACTIVE_ENEMIES = 0;

        MW.SCORE = 0;
        MW.LIFE = 4;
        this._state = STATE_PLAYING;

        // OpaqueBatch
        var texOpaque = cc.textureCache.addImage(res.textureOpaquePack_png);
        this._texOpaqueBatch = new cc.SpriteBatchNode(texOpaque);
        this._sparkBatch = new cc.SpriteBatchNode(texOpaque);
        if (cc.sys.isNative) this._sparkBatch.setBlendFunc(cc.SRC_ALPHA, cc.ONE);
        this.addChild(this._texOpaqueBatch);
        this.addChild(this._sparkBatch);

        // TransparentBatch
        var texTransparent = cc.textureCache.addImage(res.textureTransparentPack_png);
        this._texTransparentBatch = new cc.SpriteBatchNode(texTransparent);
        this.addChild(this._texTransparentBatch);

        winSize = cc.director.getWinSize();
        this._levelManager = new LevelManager(this);

        this.screenRect = cc.rect(0, 0, winSize.width, winSize.height + 10);

        // score
        /*
        //
        // The Intel XDK generated this code snippet for you
        //
        // To use it in your project, follow the
        //   instructions in comments below
        //

        // Make sure you had preloaded the assets
        //   Call this preload function when loading the game
        //   TODO: merge the preload array with your existing one
        cc.LoaderScene.preload([
            'test-assets/RealGame/MoonWarriors/arial-14.fnt',
            ]);
        // End of preloading assets

        // This function demonstrates how to create a text label using bitmap font
        function demoBitmapFontUsage (parentCocosLayer) {
            // TODO: change 'Hello World' to your own label text
            var fontLabel = new cc.LabelBMFont('Hello World', 'asset/arial-14.fnt', 24);

            // TODO: change the position
            fontLabel.setPosition(100, 100);

            parentCocosLayer.addChild(fontLabel);
        }
        */
        this.lbScore = new cc.LabelBMFont('Score: 0', res.arial_14_fnt);
        this.lbScore.attr({
            anchorX: 1,
            anchorY: 0,
            x: winSize.width - 5,
            y: winSize.height - 30,
            scale: MW.SCALE
        });
        this.lbScore.textAlign = cc.TEXT_ALIGNMENT_RIGHT;
        this.addChild(this.lbScore, 1000);

        // ship life
        var life = new cc.Sprite('#ship03.png');
        life.attr({
            scale: 0.6,
            x: 30,
            y: MW.HEIGHT - 30
        });
        this._texTransparentBatch.addChild(life, 1, 5);

        // ship Life count
        this._lbLife = new cc.LabelTTF('0', 'Arial', 20);
        this._lbLife.x = 60;
        this._lbLife.y = MW.HEIGHT - 25;
        this._lbLife.color = cc.color(255, 0, 0);
        this.addChild(this._lbLife, 1000);

        // ship
        this._ship = new Ship();
        this._texTransparentBatch.addChild(this._ship, this._ship.zOrder, MW.UNIT_TAG.PLAYER);

        // explosion batch node
        /*
        //
        // The Intel XDK generated this code snippet for you
        //
        // To use it in your project, follow the
        //   instructions in comments below
        //

        // Make sure you had preloaded the assets
        //   Call this preload function when loading the game
        //   TODO: merge the preload array with your existing one
        cc.LoaderScene.preload([
            'asset/explosion.plist',
            'asset/explosion.png',
            ]);
        // End of preloading assets

        // This function demonstrates how to load a spritesheet animation
        function demoSpritesheetAnimation (parentCocosLayer) {
            cc.SpriteFrameCache.getInstance().addSpriteFrames('asset/explosion.plist');
            var spriteSheet = cc.SpriteBatchNode.create('asset/explosion.png');
            parentCocosLayer.addChild(spriteSheet);

            // This array is holding all the frame names of this animation
            // TODO: add/remove the items in this array
            var frameNames = [
                'explosion_01.png',
                'explosion_02.png',
                'explosion_03.png',
                'explosion_04.png',
                'explosion_05.png',
                'explosion_06.png',
                'explosion_07.png',
                'explosion_08.png',
                'explosion_09.png',
                'explosion_10.png',
                'explosion_11.png',
                'explosion_12.png',
                'explosion_13.png',
                'explosion_14.png',
                'explosion_15.png',
                'explosion_16.png',
                'explosion_17.png',
                'explosion_18.png',
                'explosion_19.png',
                'explosion_20.png',
                'explosion_21.png',
                'explosion_22.png',
                'explosion_23.png',
                'explosion_24.png',
                'explosion_25.png',
                'explosion_26.png',
                'explosion_27.png',
                'explosion_28.png',
                'explosion_29.png',
                'explosion_30.png',
                'explosion_31.png',
                'explosion_32.png',
                'explosion_33.png',
                'explosion_34.png',
                'explosion_35.png',
                ];

            var animmationFramesArray = [];
            // Put every frame into the array
            frameNames.forEach(function (name) {
                var frame = cc.SpriteFrameCache.getInstance().getSpriteFrame(name);
                animmationFramesArray.push(frame);
            });

            // TODO: change the speed of animation
            var animation = cc.Animation.create(animmationFramesArray, 1 / frameNames.length);
            var sprite = cc.Sprite.createWithSpriteFrameName(frameNames[0]);
            var size = cc.Director.getInstance().getWinSize();
            sprite.setPosition(size.width / 2, size.height / 2);
            sprite.runAction(cc.RepeatForever.create(cc.Animate.create(animation)));
            spriteSheet.addChild(sprite);
        }

        */
        cc.spriteFrameCache.addSpriteFrames(res.explosion_plist);
        var explosionTexture = cc.textureCache.addImage(res.explosion_png);
        // Adds a sprite batch node.
        this._explosions = new cc.SpriteBatchNode(explosionTexture);
        this._explosions.setBlendFunc(cc.SRC_ALPHA, cc.ONE);
        // Adds spriteSheet in this layer.
        this.addChild(this._explosions);
        Explosion.sharedExplosion();

        if (cc.sys.capabilities.hasOwnProperty('keyboard'))
            cc.eventManager.addListener({
                event: cc.EventListener.KEYBOARD,
                onKeyPressed: function (key, event) {
                    MW.KEYS[key] = true;
                },
                onKeyReleased: function (key, event) {
                    MW.KEYS[key] = false;
                }
            }, this);

        if ('mouse' in cc.sys.capabilities)
            cc.eventManager.addListener({
                event: cc.EventListener.MOUSE,
                onMouseMove: function (event) {
                    if (event.getButton() == cc.EventMouse.BUTTON_LEFT)
                        event.getCurrentTarget().processEvent(event);
                }
            }, this);

        if (cc.sys.capabilities.hasOwnProperty('touches')) {
            cc.eventManager.addListener({
                prevTouchId: -1,
                event: cc.EventListener.TOUCH_ALL_AT_ONCE,
                onTouchesMoved: function (touches, event) {
                    var touch = touches[0];
                    if (this.prevTouchId != touch.getID())
                        this.prevTouchId = touch.getID();
                    else event.getCurrentTarget().processEvent(touches[0]);
                }
            }, this);
        }

        // schedule
        this.scheduleUpdate();
        this.schedule(this.scoreCounter, 1);

        if (MW.SOUND)
            cc.audioEngine.playMusic(res.bgMusic_mp3, true);

        g_sharedGameLayer = this;

        //pre set
        Bullet.preSet();
        Enemy.preSet();
        HitEffect.preSet();
        SparkEffect.preSet();
        Explosion.preSet();
        BackSky.preSet();
        BackTileMap.preSet();

        this.initBackground();

        return true;
    },

    scoreCounter: function () {
        if (this._state == STATE_PLAYING) {
            this._time++;
            this._levelManager.loadLevelResource(this._time);
        }
    },

    processEvent: function (event) {
        if (this._state == STATE_PLAYING) {
            var delta = event.getDelta();
            var curPos = cc.p(this._ship.x, this._ship.y);
            curPos = cc.pAdd(curPos, delta);
            curPos = cc.pClamp(curPos, cc.p(0, 0), cc.p(winSize.width, winSize.height));
            this._ship.x = curPos.x;
            this._ship.y = curPos.y;
            curPos = null;
        }
    },

    update: function (dt) {
        if (this._state == STATE_PLAYING) {
            this.checkIsCollide();
            this.removeInactiveUnit(dt);
            this.checkIsReborn();
            this.updateUI();
            this._movingBackground(dt);
        }
    },
    checkIsCollide: function () {
        var selChild, bulletChild;
        // check collide
        var i, locShip = this._ship;
        for (i = 0; i < MW.CONTAINER.ENEMIES.length; i++) {
            selChild = MW.CONTAINER.ENEMIES[i];
            if (!selChild.active)
                continue;

            for (var j = 0; j < MW.CONTAINER.PLAYER_BULLETS.length; j++) {
                bulletChild = MW.CONTAINER.PLAYER_BULLETS[j];
                if (bulletChild.active && this.collide(selChild, bulletChild)) {
                    bulletChild.hurt();
                    selChild.hurt();
                }
            }
            if (this.collide(selChild, locShip)) {
                if (locShip.active) {
                    selChild.hurt();
                    locShip.hurt();
                }
            }
        }

        for (i = 0; i < MW.CONTAINER.ENEMY_BULLETS.length; i++) {
            selChild = MW.CONTAINER.ENEMY_BULLETS[i];
            if (selChild.active && this.collide(selChild, locShip)) {
                if (locShip.active) {
                    selChild.hurt();
                    locShip.hurt();
                }
            }
        }
    },
    removeInactiveUnit: function (dt) {
        var i, selChild, children = this._texOpaqueBatch.children;
        for (i in children) {
            selChild = children[i];
            if (selChild && selChild.active)
                selChild.update(dt);
        }

        children = this._sparkBatch.children;
        for (i in children) {
            selChild = children[i];
            if (selChild && selChild.active)
                selChild.update(dt);
        }

        children = this._texTransparentBatch.children;
        for (i in children) {
            selChild = children[i];
            if (selChild && selChild.active)
                selChild.update(dt);
        }
    },
    checkIsReborn: function () {
        var locShip = this._ship;
        if (MW.LIFE > 0 && !locShip.active) {
            locShip.born();
        } else if (MW.LIFE <= 0 && !locShip.active) {
            this._state = STATE_GAMEOVER;
            // XXX: needed for JS bindings.
            this._ship = null;
            this.runAction(cc.sequence(
                cc.delayTime(0.2),
                cc.callFunc(this.onGameOver, this)
            ));
        }
    },
    updateUI: function () {
        if (this._tmpScore < MW.SCORE) {
            this._tmpScore += 1;
        }
        this._lbLife.setString(MW.LIFE + '');
        this.lbScore.setString('Score: ' + this._tmpScore);
    },
    collide: function (a, b) {
        var ax = a.x,
            ay = a.y,
            bx = b.x,
            by = b.y;
        if (Math.abs(ax - bx) > MAX_CONTAINT_WIDTH || Math.abs(ay - by) > MAX_CONTAINT_HEIGHT)
            return false;

        var aRect = a.collideRect(ax, ay);
        var bRect = b.collideRect(bx, by);
        return cc.rectIntersectsRect(aRect, bRect);
    },
    initBackground: function () {
        this._backSky = BackSky.getOrCreate();
        this._backSkyHeight = this._backSky.height;

        this.moveTileMap();
        this.schedule(this.moveTileMap, 5);
    },
    moveTileMap: function () {
        var backTileMap = BackTileMap.getOrCreate();
        var ran = Math.random();
        backTileMap.x = ran * 320;
        backTileMap.y = winSize.height;
        var move = cc.moveBy(ran * 2 + 10, cc.p(0, -winSize.height - backTileMap.height));
        var fun = cc.callFunc(function () {
            backTileMap.destroy();
        }, this);
        backTileMap.runAction(cc.sequence(move, fun));
    },

    _movingBackground: function (dt) {
        var movingDist = 16 * dt; // background's moving rate is 16 pixel per second

        var locSkyHeight = this._backSkyHeight,
            locBackSky = this._backSky;
        var currPosY = locBackSky.y - movingDist;
        var locBackSkyRe = this._backSkyRe;

        if (locSkyHeight + currPosY <= winSize.height) {
            if (locBackSkyRe)
                throw 'The memory is leaking at moving background';
            locBackSkyRe = this._backSky;
            this._backSkyRe = this._backSky;

            //create a new background
            this._backSky = BackSky.getOrCreate();
            locBackSky = this._backSky;
            locBackSky.y = currPosY + locSkyHeight - 5;
        } else
            locBackSky.y = currPosY;

        if (locBackSkyRe) {
            //locBackSkyRe
            currPosY = locBackSkyRe.y - movingDist;
            if (currPosY + locSkyHeight < 0) {
                locBackSkyRe.destroy();
                this._backSkyRe = null;
            } else
                locBackSkyRe.y = currPosY;
        }
    },

    onGameOver: function () {
        cc.audioEngine.stopMusic();
        cc.audioEngine.stopAllEffects();
        var scene = new cc.Scene();
        scene.addChild(new GameOver());
        cc.director.runScene(new cc.TransitionFade(1.2, scene));
    }
});

GameLayer.scene = function () {
    var scene = new cc.Scene();
    var layer = new GameLayer();
    scene.addChild(layer, 1);
    return scene;
};

GameLayer.prototype.addEnemy = function (enemy, z, tag) {
    this._texTransparentBatch.addChild(enemy, z, tag);
};

GameLayer.prototype.addExplosions = function (explosion) {
    this._explosions.addChild(explosion);
};

GameLayer.prototype.addBulletHits = function (hit, zOrder) {
    this._texOpaqueBatch.addChild(hit, zOrder);
};

GameLayer.prototype.addSpark = function (spark) {
    this._sparkBatch.addChild(spark);
};

GameLayer.prototype.addBullet = function (bullet, zOrder, mode) {
    this._texOpaqueBatch.addChild(bullet, zOrder, mode);
};