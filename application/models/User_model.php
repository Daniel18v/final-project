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
                foreach ($query as $key => $value) {
                    $recover_data[$key] = $value;
                }
                $this->session->set_userdata($recover_data);

                return 200; // Logued.
            } else {
                return 420; // Pass invalid.
            }
        } else {
            return 418; // User invalid.
        }
    }

    function signup($data) {
        $this->db->insert('users', $data);
        return $data;
    }

    function select_user($user) {
        $this->db->select()->where('user', $user);
        $query = $this->db->get('users');
        return $query->result_array()[0];
    }

    function check_user($user) {
        if ($user === NULL) {
            return 414;
        }
        $this->db->where('user', $user)->from('users');
        $query = $this->db->get();

        if ($query->num_rows() > 0) {
            return $query->result_array()[0]['user'];
        } else {
            return 414;
        }
    }

    function check_mail($mail) {
        if ($mail === NULL) {
            return 414;
        }
        $this->db->where('mail', $mail)->from('users');
        $query = $this->db->get();

        if ($query->num_rows() > 0) {
            return $query->result_array()[0]['mail'];
        } else {
            return 414;
        }
    }

    function add_coins($data, $user) {
        $this->db->select(array('bird_coins'))
                 ->where('user', $user)
                 ->get_compiled_select('users', FALSE);
        $coins_user = (int) $this->db->get()->result_array()[0]['bird_coins'];
        $data['bird_coins'] += $coins_user;
        $this->db->update('users', $data, array('user' => $user));
        $this->session->set_userdata('bird_coins', $data['bird_coins']);
        return $data['bird_coins'];
    }

    function buy_game($data, $user) {
        $this->db->select(array('bird_coins', 'games'))
                 ->where('user', $user)
                 ->get_compiled_select('users', FALSE);
        $rs = $this->db->get()->result_array()[0];
        $coins_user = (int) $rs['bird_coins'];
        $games = explode(',', $rs['games']);

        $this->db->select('bird_coins');
        $coins_game = (int) $this->db->get_where('roms', $data)->result_array()[0]['bird_coins'];
        $coins_user -= $coins_game;
        if ($coins_user > 0) {
            array_push($games, $data['id']);

            $this->db->update('users', array('bird_coins' => $coins_user, 'games' => implode(",",$games)), array('user' => $user));
            $this->session->set_userdata('games', implode(",",$games));
            $this->session->set_userdata('bird_coins', $coins_user);
            return json_encode(array("status" => 200, "coins" => $coins_user));
        } else {
            return json_encode(array("status" => 303, "coins" => (-1 * $coins_user)));
        }
    }

    function update_profile($data) {
        $this->db->where('mail', $data['mail']);
        unset($data['mail']);
        $this->db->update('users', $data);
    }

    function get_games($user) {
        $this->db->select(array('games'))
            ->where('user', $user)
            ->get_compiled_select('users', FALSE);
        return $this->db->get()->result_array()[0]['games'];
    }
    function get_roms($where) {
        $this->db->select(array('title', 'image', 'slug', 'type'))
            ->where($where)
            ->get_compiled_select('roms', FALSE);
        return $this->db->get()->result_array();
    }
}
