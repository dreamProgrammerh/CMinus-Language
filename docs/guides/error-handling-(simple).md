# **New Error System Explained Like Your 5**

## **The Problem We're Solving**
Imagine asking your friend to buy you a coffee. Three things can happen:
1. **Success** - You get coffee ☕
2. **Error** - Store is closed 🚫
3. **Crash** - Friend gets hit by a car 💥

Most programming languages only handle #1 well. Ours handles all three beautifully.

## **The 3 Magic Words of Our System**

### **1. `boil` = "Something went wrong!"**
```c
Coffee buyCoffee(float money) {
    boil StoreClosed when store.isClosed();
    boil NoMoney when money < 5;
    return Coffee();  // Success!
}
```
- Like saying "If store is closed, TELL ME!"
- Not a crash, just information

### **2. `flow` = "Here's Plan B"**
```c
let coffee = buyCoffee?(10) flow {
    case StoreClosed -> makeInstantCoffee(),
    case NoMoney -> borrowFromFriend(),
    default -> drinkWater()  // Any other error
};
```
- Like saying "If store closed, make instant coffee instead"
- Always gives you SOME drink (never empty-handed)

### **3. `bucket` = "Emergency Plan"**
```c
bucket StoreClosed e {
    println("Store closed at " + e.time);
    yield makeTea();  // Give tea instead
}

let drink = buyCoffee!(10);  // Uses bucket if store closed
```
- Like having an emergency kit
- Applies to EVERY coffee purchase in this area

## **The 5 Ways to Ask for Coffee**

| Symbol | Meaning | Real Life Example |
|--------|---------|-------------------|
| `buyCoffee()` | "Try, tell me if fails" | "Try to buy, call me if any problem" |
| `buyCoffee?()` | "Try, give me water if fails" | "Try to buy, bring water if can't" |
| `buyCoffee!()` | "Try, use emergency kit if fails" | "Try to buy, use bucket plan if fails" |
| `buyCoffee!!()` | "MUST succeed or crash" | "BUY OR DIE TRYING" |
| `buyCoffee??()` | "Try, tell me exactly what happened" | "Try, report success/failure in detail" |

## **Real Example: Ordering Food**
```c
errtype FoodError() {
    RestaurantClosed(str<4> time),
    NoDeliveryAvailable(),
    PaymentFailed(string reason)
};

FastFood orderPizza() boils FoodError {
    boil RestaurantClosed("9PM") when time > 9;
    boil NoDeliveryAvailable when driverCount == 0;
    boil PaymentFailed("Card declined") when !card.valid();
    
    return Pizza();
}

main {
    // Setup emergency plans
    bucket RestaurantClosed e {
        yield orderBurger();  // Burger if pizza place closed
        // use continue; to continue execution with no return
    }
    
    bucket<t> anyError {
        yield eatLeftovers();  // Leftovers for ANY error
    }
    
    // Different ordering strategies:
    
    // 1. Casual try
    let meal = orderPizza?() flow orderNoodles;
    // "Try pizza, get noodles if fails"
    
    // 2. Use emergency plans  
    let meal = orderPizza!();
    // "Try pizza, use buckets if fails"
    
    // 3. Must have pizza
    let meal = orderPizza!!();
    // "I MUST HAVE PIZZA OR I CRY"
    
    // 4. Want details
    let result = orderPizza??();
    // "Tell me if succeeded or exactly why failed"
}
```

## **The Guard (`g`) - Safety First!**
```c
g? {
    let pizza = orderPizza!!();  // Must succeed
    eat(pizza);
    playGames();
} flow {
    case FoodError -> {
        println("No food, going to sleep");
        continue; // continue to next execution without any value (default value)
        // yield value; continue with value as replacement
        // retry; try again
    }
} finally {
    // ALWAYS runs, even if error with access to guard scope
    free(pizza) when pizza != null;
    washDishes();
    brushTeeth();
};
```
- Like saying "Do all this, but clean up NO MATTER WHAT"
- Even crashes get cleaned up

## **Why This Beats Old Systems:**

### **Old Way (Try/Catch):**
```c++
try {
    // Not clear witch can cause error and what to do about it
    Pizza* p = orderPizza();
    eat(p);
} catch (Exception e) {
    // What kind of error? No idea!
    std::cout << "Error" << std::endl;
    // maybe we want to retry or use something else as value instead
} finally {
  // How to free pizza now?
  // p is not available here
  washDishes();
  brushTeeth();
}
```

**Problems:**
- Not clear where errors can occur
- Can't see at call site if errors are handled
- No way to continue execution with value or retry the same operation
- Errors can slip through silently
- Cleanup can't clean try block

### **New Way:**
```c
let meal = orderPizza!()  // ! = "use buckets or crash"
         flow eatSandwich  // Backup plan
         finally -> washDishes();  // Always cleanup
```

**Advantages:**
- `!` shows it's guarded
- `flow` shows backup plan
- `finally` guarantees cleanup

## **Simple Rules to Remember:**

1. **Normal Call** when you don't need error handling (error will go up stack)
2. **Use `?`** when you don't really care (quick tests)
3. **Use `!`** for real code (with `bucket` backups)
4. **Use `!!`** for critical stuff (must work)
5. **Use `??`** when you need error details
6. **Wrap in `g?`** when you need guard a block or none function
7. **Add `bucket`** for shared emergency plans in scope

## **The Magic Sauce:**
**The compiler checks EVERY possible error path.** No surprises at 3 AM when your app crashes because "some error wasn't handled."

## **TL;DR Version:**
Ours system makes errors:
1. **Visible** (see `!` or `?` at call)
2. **Handled** (always have backup with `flow`)
3. **Emergency** (always have emergency plan with `bucket`)
4. **Clean** (always cleanup with `finally`)
5. **Safe** (compiler checks everything)

It's like having seatbelts, airbags, AND a parachute in your car. You hope not to need them, but they're there and GUARANTEED to work.

---

**Bottom line:** Your friend's code will crash less, be easier to fix, and won't leak resources. It's error handling for adults who want to sleep well at night. 😴