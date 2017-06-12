Cocos2d-HTML5 3.1 Font and Spritesheet Animation Sample with Google Play Game Service Plugin
============================================================================================

See [LICENSE.md](<LICENSE.md>) for license terms and conditions.

This sample is used to show the usage of Font and Spritesheet Animation in
Cocos2d-JS 3.1 engine with the Intel XDK. The assets and code are created from
[Moon Warriors][1] by Cocos2d-x.org.

[1]: <https://github.com/cocos2d/cocos2d-js/tree/develop/samples/js-moonwarriors>

### Application Files

-   asset/
-   frameworks/
-   src/
-   config/
-   app.js
-   AboutLayer.js
-   Background.js
-   Bullet.js
-   Effect.js
-   Enemy.js
-   Explosion.js
-   GameController.js
-   GameControllMenu.js
-   GameLayer.js
-   GameOver.js
-   HitEffect.js
-   LevelManager.js
-   Resource.js
-   SettingsLayer.js
-   Ship.js
-   SparkEffect.js
-   SysMenu.js
-   index.html
-   main.js
-   project.json

### Intel(R) XDK

This sample is part of the Intel XDK.
Download the Intel XDK at <http://xdk.intel.com>.


Overview
--------

`www/main.js` is the entrance of the game which loads the resources and switches
to the start scene.

### Game Asset Manager

Game Asset Manager (GAM) is a tool for previewing and creating code snippets for
various assets commonly used in game projects. By creating or importing a game
project in Intel XDK, you will find GAM panel on the left side of "DEVELOP" tab.

With Game Asset Manager, you can generate code snippet to use font and
spritesheet animation in Cocos2d-js.

### Preload Bitmap Font & Spritesheets

In Cocos2d, all resources should be loaded by `cc.LoaderScene.preload(resources,
callback)` or `cc.Loader.preload(resources, callback)` if you don't need to
create a loader scene.

In this sample, resources are loaded separately in `www/main.js`,
`www/src/GameLayer.js` and `www/src/SysMenu.js`. See these files for detailed
usage of resource loading.

### Usage of Bitmap Font

Cocos2d-js supports Bitmap Font in .fnt format. Tools like [Glyph Designer][3]
and [Bitmap Font Generator][4] can help to genrate bitmap font.

[3]: <https://71squared.com/glyphdesigner>
[4]: <http://www.angelcode.com/products/bmfont/>

In Cocos2d, it's very easy to use bitmap font to display text by `new
cc.LabelBMFont(text, bitmap_fnt)`. For example, to render "Hello World" with
bitmap font defined in `www/asset/arial-14.fnt`, Game Asset Manager can help
generate following code snippets:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    'asset/arial-14.fnt',
    ]);
// End of preloading assets

// This function demonstrates how to create a text label using bitmap font
function demoBitmapFontUsage (parentCocosLayer) {
    // TODO: change "Hello World" to your own label text
    var fontLabel = new cc.LabelBMFont("Hello World", "asset/arial-14.fnt", 24);

    // TODO: change the position
    fontLabel.setPosition(100, 100);

    parentCocosLayer.addChild(fontLabel);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Simply paste the code to your project and make some changes. See
`www/src/GameLayer.js` for real usage of bitmap font.

### Usage of Spritesheets

Spritesheet in Cocos2d is a packed image containing various frames used in the
game. Usually all frames within the spritesheet ar defined in an XML format file
(.plist) with a reference to the image. Each frame is an individual image that
can be rendered in Cocos2d.

In Cocos2d, all resources should be loaded by `cc.LoaderScene.preload(resources,
callback)` or `cc.Loader.preload(resources, callback)` if you don't need a
loader scene.

Spritesheet should be added into cache before used to create a sprite with
`cc.SpriteFrameCache.getInstance().addSpriteFrames(spritesheet_plist)`. And use
`cc.Sprite.createWithSpriteFrameName(frameName)` to create a sprite from a
frame.

With a list of frames, you can create a sprite animation through
`cc.Animation.create(frames, frameRate)`.

In this sample, to display a "explosion" animation defined in
`asset/explosion.plist`, Game Asset Manager can generate following code
snippets:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
    cc.spriteFrameCache.addSpriteFrames("asset/explosion.plist");
    var spriteSheet = new cc.SpriteBatchNode("asset/explosion.png");
    parentCocosLayer.addChild(spriteSheet);

    // This array is holding all the frame names of this animation
    // TODO: add/remove the items in this array
    var frameNames = [
        'explosion_01',
        'explosion_02',
        'explosion_03',
        'explosion_04',
        'explosion_05',
        'explosion_06',
        'explosion_07',
        'explosion_08',
        'explosion_09',
        'explosion_10',
        'explosion_11',
        'explosion_12',
        'explosion_13',
        'explosion_14',
        'explosion_15',
        'explosion_16',
        'explosion_17',
        'explosion_18',
        'explosion_19',
        'explosion_20',
        'explosion_21',
        'explosion_22',
        'explosion_23',
        'explosion_24',
        'explosion_25',
        'explosion_26',
        'explosion_27',
        'explosion_28',
        'explosion_29',
        'explosion_30',
        'explosion_31',
        'explosion_32',
        'explosion_33',
        'explosion_34',
        'explosion_35',
        ];

    var animmationFramesArray = [];
    // Put every frame into the array
    frameNames.forEach(function (name) {
        var frame = cc.spriteFrameCache.getSpriteFrame(name);
        animmationFramesArray.push(frame);
    });

    // TODO: change the speed of animation
    var animation = new cc.Animation(animmationFramesArray, 1 / frameNames.length);
    var sprite = cc.Sprite.createWithSpriteFrameName(frameNames[0]);
    var size = cc.winSize;
    sprite.setPosition(size.width / 2, size.height / 2);
    sprite.runAction(new cc.RepeatForever(new cc.Animate(animation)));
    spriteSheet.addChild(sprite);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Simply paste these code snippets into your project and make changes according to
your needs. See `www/src/GameLayer.js` for detailed usage.


Google Play Game Service Plugin
-------------------------------

[Google Play Game Service Plugin][5] provides leaderboard and achievement API
for games. You can find and enable this plugin in Intel XDK under **PROJECTS**
\> **CORDOVA HYBRID MOBILE APP SETTINGS** \> **PLUGINS** \> **Featured & Custom
Cordova Plugins**. Turn to `www/src/app.js` and `www/src/GameOver.js` for the
sample usage of the plugin.

[5]: <https://github.com/01org/cordova-google-play-games-services>
