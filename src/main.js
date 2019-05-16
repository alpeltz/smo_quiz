// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import { VPopover } from 'v-tooltip'
import VModal from 'vue-js-modal'
import App from './App'

Vue.config.productionTip = false

Vue.component('v-popover', VPopover)
Vue.use(VModal)

/* eslint-disable no-new */
new Vue({
  el: '#app',
  components: { App },
  template: '<App/>'
})
