<?php

class MY_Exceptions extends CI_Exceptions {
    public function __construct() {
        parent::__construct();
        $this->ci =& get_instance();
    }

    public function show_404($page = '', $log_error = TRUE) {
        if ($log_error) {
            log_message('error', $page);
        }
        echo $this->ci->templates->render("errors::404", ['sess' => $this->ci]);
        exit(4); // EXIT_UNKNOWN_FILE
    }

    public function show_403($page = '', $log_error = TRUE) {
        if ($log_error) {
            log_message('error', $page);
        }
        echo $this->ci->templates->render("errors::403", ['sess' => $this->ci]);
        exit(4); // EXIT_UNKNOWN_FILE
    }
}