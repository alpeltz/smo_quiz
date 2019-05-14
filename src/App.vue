<template>
  <div id="app">
    <h1>Learn The Shrines</h1>

    <div class="options">
      <!--<div class="dropdown">
        <dl>
          <dt>Max Choices</dt>
          <dd>{{ options.chooseFrom }}</dd>
        </dl>

        <div class="dropdown-content">
          <div class="option" v-for="num in 7" :key="num" :class="{active: num + 1 === options.chooseFrom}" @click="changeOption('chooseFrom', num + 1)">
            {{ num + 1 }}
          </div>
        </div>
      </div>-->

      <div class="dropdown">
        <dl>
          <dt>Difficulty</dt>
          <dd class="small">{{ options.difficulty | capitalize }}</dd>
        </dl>

        <div class="dropdown-content">
          <div class="option" v-for="difficulty in difficulties" :key="difficulty" :class="{active: difficulty === options.difficulty}" @click="changeOption('difficulty', difficulty)">
            {{ difficulty | capitalize }}
          </div>
        </div>
      </div>

      <!--<dl class="toggle" @click="toggleOption('includeDLC')">
        <dt>DLC</dt>
        <dd>{{ options.includeDLC ? 'ON' : 'OFF' }}</dd>
      </dl>-->

      <dl class="toggle" @click="toggleOption('soundOn', false)">
        <dt>Sounds</dt>
        <dd>{{ options.soundOn ? 'ON' : 'OFF' }}</dd>
      </dl>
    </div>

    <score-card v-if="total.out_of > 0" :score="total" @reset="resetScore"></score-card>

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
import ScoreCard from './components/ScoreCard'

export default {
  name: 'App',
  components: {
    Quiz,
    ScoreCard
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
      },
      difficulties: ['easy', 'normal', 'hard']
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
    },
    toggleOption (option, newQ = true) {
      this.options[option] = !this.options[option]
      if (newQ) {
        this.$refs.quiz.newQuestion()
      }
    },
    changeOption (option, value) {
      if (this.options[option] !== value && this.resetScore()) {
        this.options[option] = value
        this.$refs.quiz.newQuestion()
      }
    }
  },
  filters: {
    capitalize (value) {
      return value.charAt(0).toUpperCase() + value.slice(1)
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

.score-table {
  margin: 1em auto 0;
  max-width: 600px;
}

.score-table dl,
.score-table dt,
.score-table dd,
.score-table ul,
.score-table li {
  margin: 0;
  padding: 0;
}

.score-table ul {
  padding-right: 30px;
  list-style: none;
}

.score-table dl {
  padding-top: 2px;
}

.score-table dt {
  clear: left;
  float: left;
  padding-right: 1em;
  width: 50%;
  text-align: left;
}

.score-table dd {
  position: relative;
  float: right;
  width: 50%;
}

.score-table li::after {
  clear: both;
  display: table;
  content: '';
}

.score-table .bar {
  display: block;
  background: rgba(255,255,255,.8);
}

.score-table .percent {
  position: absolute;
  padding-left: 5px;
  left: 100%;
  top: 50%;
  transform: translateY(-50%);
}

a {
  color: inherit;
}

.score.perfect {
  font-weight: bold;
  color: #98d098;
}

.score.perfect a {
  font-weight: normal;
  color: #fff;
}

footer {
  margin-top: 50px;
  font-weight: bold;
  color: rgba(255, 255, 255, .5);
}

footer code {
  display: block;
}

.options {
  display: flex;
  position: fixed;
  top: 10px;
  left: 10px;
  font-weight: bold;
  font-size: 16px;
}

@media (max-width: 600px) {
  .options {
    font-size: 12px;
  }
}

.options dl {
  display: flex;
  flex-direction: column;
  float: left;
  margin: 0;
  padding: 5px 0;
  width: 80px;
  height: 80px;
  border: solid 1px #fff;
  background: #060903;
  text-align: center;
  justify-content: center;
}

@media (max-width: 600px) {
  .options dl {
    width: 60px;
    height: 60px;
  }
}

.options dl + dl,
.options .dropdown + dl,
.options dl + .dropdown > dl,
.options .dropdown + .dropdown > dl {
  border-left: 0;
}

.options dt {
  padding: 0 5px;
  margin: 0;
  font-size: 62.5%;
  text-transform: uppercase;
}

.options dd {
  margin: 0;
  font-size: 175%;
}

.options dd.small {
  margin: 0;
  font-size: 100%;
}

.options dl.toggle,
.options .dropdown dl {
  cursor: pointer;
}

.options dl.toggle:hover,
.options .dropdown dl:hover {
  background: #fff;
  color: #060903;
}

.options .dropdown {
  position: relative;
}

.options .dropdown:hover .dropdown-content {
  display: block;
}

.options .dropdown .dropdown-content {
  display: none;
  position: absolute;
  top: 100%;
  left: 0;
  right: 0;
  padding: 2px 8px;
  border: solid 1px #fff;
  border-top: 0;
  background: #060903;
  text-align: left;
}

.options .dropdown .option {
  cursor: pointer;
  color: rgba(255,255,255, 0.5);
}

.options .dropdown .option {
  cursor: pointer;
}

.options .dropdown .option:hover,
.options .dropdown .option.active {
  color: #fff;
}
</style>
