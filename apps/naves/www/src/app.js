/**
 * Initialize Cordova plugins
 * For more Corova plugins, please go to [Cordova Plugin Registry](http://plugins.cordova.io/#/).
 * In Intel XDK, you can enable / disable / add Cordova Plugins on
 * Projects Tab
 *  -> Cordova 3.x Hybrid Mobile App Settings
 *     -> Plugins and Permissions
 */

var ACHIEVEMENTS = {
    'FINISH_GAME_ONCE': 'FINISH_GAME_ONCE_ACHIEVEMENT_ID'
    // add other achievement id below
};

var LEADERBOARDS = {
    'SCORE': 'SCORE_LEADERBOARD_ID'
    // add other leaderboard id below
};

var app = {
    // Listen to deviceready event which is fired when Cordova plugins are ready
    bindEvents: function() {
        document.addEventListener('deviceready', this.init.bind(this));
    },
    init: function() {
        // Call splashscreen API to hide the splash.
        navigator.splashscreen.hide();

        if (self.GooglePlayGamesPlugin) {
            app._show('Google Play Games Plugin is installed.');
        } else {
            app._show(
                'You don\'t have Google Play Games Plugin installed. Google Play Games Plugin is not available in Intel XDK Emulator. Please test your app on real device.'
            );
        }
    },
    // Connect to plugin
    connect: function() {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Connecting ...');
        GooglePlayGamesPlugin.connect(function() {
            // success
            app._show('Successfully connected to plugin');
        }, function() {
            app._show('Failed to connect to Google Play Games plugin');
        });
    },
    // Authenticate with Google Play Games service
    auth: function() {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Authenticating ...');
        GooglePlayGamesPlugin.authenticate(function() {
            // success
            app._show('Authentication success!');
        }, function() {
            app._show('Failed to authenticate with Google Play Games Service.');
        });
    },
    logout: function() {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Logging out ...');
        GooglePlayGamesPlugin.logout(function(res) {
            app._show('Successfully logout');
        }, function(res) {
            app._show('Failed to logout');
        });
    },
    showAchievements: function() {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Achievements');
        GooglePlayGamesPlugin.showAchievements(function(res) {
            app._show();
        }, function(res) {
            app._show('Failed to show leaderboard');
        });
    },
    addAchievement: function(achievement_id) {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Adding achievement ...');
        GooglePlayGamesPlugin.addAchievement(achievement_id, function(res) {
            app._show('Achievement added ' + res);
        }, function(res) {
            app._show('Failed to add achievement ' + res);
        });
    },
    showLeaderboard: function(leaderboard_id) {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Leaderboard');
        GooglePlayGamesPlugin.showLeaderboard(leaderboard_id, function(res) {
            app._show();
        }, function(res) {
            app._show('Error showing leaderboard ' + res);
        });
    },
    updateLeaderboardScore: function(leaderboard_id, score) {
        if (!self.GooglePlayGamesPlugin) return;
        app._show('Updating leaderboard ...');
        GooglePlayGamesPlugin.updateLeaderboardScore(leaderboard_id, score, function(res) {
            app._show('Successfully updated leaderboard');
        }, function(res) {
            app._show('Failed to update leaderboard ' + res);
        });
    },
    /**
     * Belows are private functions
     */
    _show: function(msg) {
        var overlay = document.getElementById('overlay');
        if (!overlay) {
            overlay = document.createElement('div');
            overlay.setAttribute('id', 'overlay');
            overlay.innerHTML = '<div><div id="msg"></div><div><button id="connect">Connect</button><button id="auth">Auth</button><button id="leaderboard">LeaderBoard</button><button id="achievements">Achievements</button><button id="hide">Hide</button></div></div>';
            document.body.appendChild(overlay);

            var hide = document.getElementById('hide');
            hide.addEventListener('click', function() {
                overlay.classList.remove('show');
            });

            var connect = document.getElementById('connect');
            connect.addEventListener('click', function() {
                app.connect();
            });
            var auth = document.getElementById('auth');
            auth.addEventListener('click', function() {
                app.auth();
            });
            var leaderboard = document.getElementById('leaderboard');
            leaderboard.addEventListener('click', function() {
                app.showLeaderboard(LEADERBOARDS.SCORE);
            });
            var achievements = document.getElementById('achievements');
            achievements.addEventListener('click', function() {
                app.showAchievements();
            });
        }
        overlay.classList.add('show');
        if (msg) {
            var msgNode = document.getElementById('msg');
            msgNode.innerHTML = msg;
        }
    }
};

app.bindEvents();
