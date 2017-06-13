<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Errors_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller.
     */
    function __construct() {
        parent::__construct();
    }

    /**
     * Shows a 404 error.
     */
    function error404() {
        $this->output->set_status_header('404');
        echo $this->templates->render("errors::404", ['sess' => $this]);
    }

    /**
     * Shows a 403 error.
     */
    function error403() {
        $this->output->set_status_header('403');
        echo $this->templates->render("errors::403", ['sess' => $this]);
    }

}