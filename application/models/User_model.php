<?php

defined("BASEPATH") or exit("No direct script access allowed");


class User_model extends CI_Model {

    function __construct() {
        $this->load->database();
    }

    function login($data) {
        $this->db->where('user', $data['user'])->from('users');
        $query = $this->db->get();

        if ($query->num_rows() > 0) {
            $this->db->where('pass', $data['pass'])->where('user', $data['user'])->from('users');
            $query = $this->db->get();
            if ($query->num_rows() > 0) {
                $query = $query->row();
                $recover_data  = array(
                        'user' => $query->user,
                        'avatar' => $query->avatar,
                        'id' => $query->id,
                        'rol' => $query->rol
                );
                $this->session->set_userdata($recover_data);
            } else {
                $this->session->set_flashdata('error', 'El usuario no es correcto');
            }
        } else {
            $this->session->set_flashdata('error', 'El usuario no es correcto');
        }
    }

    function signup($data) {
        $this->db->insert('users', $data);
        return $data;
    }

    function check_user($id) {
        if ($id === NULL) {
            return FALSE;
        }
        $this->db->where('id', $id)->from('users');
        $query = $this->db->get();
        if ($query->num_rows() > 0) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}
