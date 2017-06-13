<?php
defined("BASEPATH") or exit("No direct script access allowed");

class UCP_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('User_model', 'mod');
    }

    public function profile() {
        echo $this->templates->render('main::UCP', ['sess' => $this]);
    }
}
