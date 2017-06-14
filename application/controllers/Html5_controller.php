<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Html5_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
    }

    /**
     * Lists all HTML5 games
     */
    public function games($game = FALSE) {
        switch (strtolower($game)) {
            case "flappybird" :
                echo $this->templates->render("games::flappy", ['sess' => $this]);
                break;
            case "galaga" :
                echo $this->templates->render("games::galaga", ['sess' => $this]);
                break;
            case "spaceship" :
                echo $this->templates->render("games::spaceship", ['sess' => $this]);
                break;
            default:
                echo $this->templates->render('main::games', ['sess' => $this]);
                break;
        }
    }

}