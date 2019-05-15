<template>
  <div id="app">
    <h1>Learn The Shrines</h1>

    <app-menu :options="options" :score="total" @reset="resetScore"></app-menu>

    <Quiz @answer="tallyScore" @reset="resetScore" :options="options" ref="quiz" />

    <footer>
      <code>v0.9.1</code>
      Created by BoyontheCob
      <br />
      <br />

      <form action="https://www.paypal.com/cgi-bin/webscr" method="post" target="_top">
        <input type="hidden" name="cmd" value="_donations" />
        <input type="hidden" name="business" value="WTZSPZL7HFLML" />
        <input type="hidden" name="currency_code" value="CAD" />
        <input type="image" src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif" border="0" name="submit" title="PayPal - The safer, easier way to pay online!" alt="Donate with PayPal button" />
        <img alt="" border="0" src="https://www.paypal.com/en_CA/i/scr/pixel.gif" width="1" height="1" />
      </form>

      <small>Pls. Help keep this free of ads</small>

    </footer>

  </div>
</template>

<script>
import Quiz from './components/Quiz'
import AppMenu from './components/AppMenu'

export default {
  name: 'App',
  components: {
    Quiz,
    AppMenu
  },
  data () {
    return {
      total: {
        score: 0,
        out_of: 0,
        by_type: {}
      },
      options: {
        soundOn: true,
        includeDLC: false,
        chooseFrom: 4,
        difficulty: 'normal'
      }
    }
  },
  methods: {
    tallyScore (correct, type) {
      this.total.out_of++
      if (this.total.by_type[type]) {
        this.total.by_type[type].out_of++
      } else {
        this.total.by_type[type] = {
          score: 0,
          out_of: 1
        }
      }

      if (correct) {
        this.total.score++
        this.total.by_type[type].score++
      }
    },
    resetScore () {
      if (this.total.out_of > 0) {
        if (window.confirm('This will permanently clear your score.')) {
          this.total = {
            score: 0,
            out_of: 0,
            by_type: {}
          }
          return true
        } else {
          return false
        }
      } else {
        this.$refs.quiz.newQuestion()
        return true
      }
    }
  }
}
</script>

<style>
*,
*:before,
*:after {
  box-sizing: border-box;
}

html {
  height: 100%;
}

body {
  display: flex;
  margin: 0;
  min-height: 100%;
  background: repeat 0 0 / 50% url('./assets/bg.jpg') #060903;
  align-items: center;
}

#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  padding: 10px;
  width: 100%;
  text-align: center;
  color: #fff;
}

@media (max-width: 1500px) {
  #app {
    padding-top: 80px;
  }
}

@media (max-width: 600px) {
  #app {
    padding-top: 60px;
  }
}

a {
  color: inherit;
}

footer {
  margin-top: 50px;
  font-weight: bold;
  color: rgba(255, 255, 255, .5);
}

footer code {
  display: block;
}
</style>
