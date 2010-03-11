<?php
// $Id$

function token_autocomplete() {
  $args = func_get_args();
  $string = implode('/', $args);

  $token_info = token_info();

  preg_match_all('/\[([^\s\]:]*):?([^\s\]]*)?\]?/', $string, $matches);
  $types = $matches[1];
  $tokens = $matches[2];

  foreach ($types as $index => $type) {
    if (!empty($tokens[$index]) || isset($token_info['types'][$type])) {
      token_autocomplete_token($type, $tokens[$index]);
    }
    else {
      token_autocomplete_type($type);
    }
  }

}

function token_autocomplete_type($string = '') {
  $token_info = token_info();
  $types = $token_info['types'];
  $matches = array();

  foreach ($types as $type => $info) {
    if (!$string || strpos($type, $string) === 0) {
      $type_key = "[{$type}:";
       $matches[$type_key] = levenshtein($type, $string);
    }
  }

  if ($string) {
    asort($matches);
  }
  else {
    ksort($matches);
  }

  $matches = drupal_map_assoc(array_keys($matches));
  drupal_json_output($matches);
}

function token_autocomplete_token($type, $string = '') {
  $token_info = token_info();
  $tokens = $token_info['tokens'][$type];
  $matches = array();

  foreach ($tokens as $token => $token_info) {
    if (!$string || strpos($token, $string) === 0) {
      // Check if the token can be continued.
      $token_key = "[{$type}:{$token}" . (empty($token_info['type']) ? ']' : ':');
      $matches[$token_key] = levenshtein($token, $string);
    }
  }

  if ($string) {
    asort($matches);
  }
  else {
    ksort($matches);
  }

  $matches = drupal_map_assoc(array_keys($matches));
  drupal_json_output($matches);
}