var res = {
    bgMusic_mp3: 'asset/Music/bgMusic.mp3',
    bgMusic_ogg: 'asset/Music/bgMusic.ogg',
    buttonEffet_mp3: 'asset/Music/buttonEffet.mp3',
    buttonEffet_ogg: 'asset/Music/buttonEffet.ogg',
    explodeEffect_mp3: 'asset/Music/explodeEffect.mp3',
    explodeEffect_ogg: 'asset/Music/explodeEffect.ogg',
    fireEffect_mp3: 'asset/Music/fireEffect.mp3', //unused
    fireEffect_ogg: 'asset/Music/fireEffect.ogg', //unused
    mainMainMusic_mp3: 'asset/Music/mainMainMusic.mp3',
    mainMainMusic_ogg: 'asset/Music/mainMainMusic.ogg',
    shipDestroyEffect_mp3: 'asset/Music/shipDestroyEffect.mp3',
    shipDestroyEffect_ogg: 'asset/Music/shipDestroyEffect.ogg',
    arial_14_fnt: 'asset/arial-14.fnt',
    arial_14_png: 'asset/arial-14.png',
    b01_plist: 'asset/b01.plist',
    b01_png: 'asset/b01.png',
    cocos2d_html5_png: 'asset/cocos2d-html5.png',
    explode_plist: 'asset/explode.plist', //unused
    explosion_plist: 'asset/explosion.plist',
    explosion_png: 'asset/explosion.png',
    flare_jpg: 'asset/flare.jpg',
    gameOver_png: 'asset/gameOver.png',
    // level01_tmx : 'asset/level01.tmx',
    loading_png: 'asset/loading.png',
    logo_png: 'asset/logo.png',
    menu_png: 'asset/menu.png',
    menuTitle_png: 'asset/menuTitle.png',
    textureOpaquePack_plist: 'asset/textureOpaquePack.plist',
    textureOpaquePack_png: 'asset/textureOpaquePack.png',
    textureTransparentPack_plist: 'asset/textureTransparentPack.plist',
    textureTransparentPack_png: 'asset/textureTransparentPack.png'
};

var g_mainmenu = [
    res.loading_png,
    res.flare_jpg,
    res.menu_png,
    res.logo_png,
    res.b01_png,
    res.b01_plist,
    res.mainMainMusic_mp3,
    res.mainMainMusic_ogg,
    res.menuTitle_png,
    res.textureTransparentPack_plist,
    res.textureTransparentPack_png
];

var g_maingame = [
    //image
    res.cocos2d_html5_png,
    res.gameOver_png,
    res.arial_14_png,
    res.explosion_png,
    res.textureOpaquePack_png,

    //tmx
    //res.level01_tmx,

    //plist
    res.explosion_plist,
    res.textureOpaquePack_plist,

    //music
    res.bgMusic_mp3,
    res.bgMusic_ogg,

    //effect
    res.buttonEffet_mp3,
    res.explodeEffect_mp3,
    res.fireEffect_mp3,
    res.shipDestroyEffect_mp3,
    res.buttonEffet_ogg,
    res.explodeEffect_ogg,
    res.fireEffect_ogg,
    res.shipDestroyEffect_ogg,

    // FNT
    res.arial_14_fnt
];